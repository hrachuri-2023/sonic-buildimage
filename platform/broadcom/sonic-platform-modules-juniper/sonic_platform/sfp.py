##
# Module containsan implementation of SONiC Platform Base API and provides
# the xcvr information
##

try:
    import os
    import time
    import mmap
    import syslog
    import struct
    import glob
    from sonic_platform_base.sonic_xcvr.sfp_optoe_base import SfpOptoeBase
except ImportError as e:
    raise ImportError(str(e) + " - required module not found")

SFP_TYPE_LIST = [
    0x3   # SFP/SFP+/SFP28 and later
]
QSFP_TYPE_LIST = [
    0xc,  # QSFP
    0xd,  # QSFP+ or later
    0x11  # QSFP28 or later
]
QSFP_DD_TYPE_LIST = [
    0x18, # QSFP-DD
    0x19, # OSFP
    0x1e
]

BASE_RES_PATH = "/sys/bus/pci/devices/0000:0d:00.0/resource0"
SYSLOG_IDENTIFIER = "SfpOptoe"
PAGE_SIZE = 4096

def log_info(msg):
    print(msg)
    syslog.openlog(SYSLOG_IDENTIFIER)
    syslog.syslog(syslog.LOG_INFO, msg)
    syslog.closelog()

def pci_mem_read(mm, offset):
    mm.seek(offset)
    read_data_stream = mm.read(4)
    reg_val = struct.unpack('I', read_data_stream)
    mem_val = str(reg_val)[1:-2]
    #   log_info "reg_val read:%x"%reg_val
    return mem_val

def pci_mem_write(mm, offset, data):
    mm.seek(offset)
    #   log_info "data to write:%x"%data
    mm.write(struct.pack('I', data))

def pci_set_value(resource, val, offset):
    fd = os.open(resource, os.O_RDWR)
    PAGE_BASE = offset & ~(PAGE_SIZE - 1)
    mm = mmap.mmap(fd, PAGE_SIZE, offset=PAGE_BASE, prot=mmap.PROT_WRITE)
    val = pci_mem_write(mm, offset, val)
    mm.close()
    os.close(fd)
    return val

def pci_get_value(resource, offset):
    fd = os.open(resource, os.O_RDWR)
    PAGE_BASE = offset & ~(PAGE_SIZE - 1)
    mm = mmap.mmap(fd, PAGE_SIZE, offset=PAGE_BASE, prot=mmap.PROT_READ)
    val = pci_mem_read(mm, offset - PAGE_BASE)
    mm.close()
    os.close(fd)
    return val

def cpld_reset():
    """
    Need to decide when to call this.
    """
    for offset in [0xf4, 0x184]:
        pci_set_value(BASE_RES_PATH, 0xFFFF, offset)

    time.sleep(3)

    # Unsetting
    for offset in [0xf4, 0x184]:
        pci_set_value(BASE_RES_PATH, 0X0000, offset)

class Sfp(SfpOptoeBase):
    """
    QFX5230 Platform specific Sfp class
    """
    EEPROM_BASE = "/sys/class/i2c-adapter/i2c-1/channel-{0}/*-0050/eeprom"
    BASE_RES_PATH = "/sys/bus/pci/devices/0000:0d:00.0/resource0"

    def __init__(self, index, slot):
        log_info('SFP Init')
        SfpOptoeBase.__init__(self)
        self._index = index
        self._slot = slot
        self._eepromPath = self.EEPROM_BASE.format(self._index)
        self._sfp_type = None
        # self.sfp_type()
        # self._driver = self.get_optoe_driver()
        self._init_sfp()

    def get_eeprom_path(self):
        paths = glob.glob(self._eepromPath)
        if paths:
            self._eepromPath = paths[0]
        return self._eepromPath
    
    def get_optoe_driver(self):
        if self._sfp_type == 'SFP':
            return 'optoe2'
        elif self._sfp_type == 'QSFP':
            return 'optoe1'
        elif self._sfp_type == 'QSFP-DD':
            return 'optoe3'
        else:
            return 'optoe'

    def pci_mem_read(self, mm, offset):
        mm.seek(offset)
        read_data_stream = mm.read(4)
        reg_val = struct.unpack('I', read_data_stream)
        mem_val = reg_val[0]
        #   log_info "reg_val read:%x"%reg_val
        return mem_val

    def pci_mem_write(self, mm, offset, data):
        mm.seek(offset)
        #   log_info "data to write:%x"%data
        mm.write(struct.pack('I', data))

    def pci_set_value(self, resource, val, offset):
        fd = os.open(resource, os.O_RDWR)
        PAGE_BASE = offset & ~(PAGE_SIZE - 1)
        mm = mmap.mmap(fd, PAGE_SIZE, offset=PAGE_BASE, prot=mmap.PROT_WRITE)
        val = self.pci_mem_write(mm, offset, val)
        mm.close()
        os.close(fd)
        return val

    def pci_get_value(self, resource, offset):
        fd = os.open(resource, os.O_RDWR)
        PAGE_BASE = offset & ~(PAGE_SIZE - 1)
        mm = mmap.mmap(fd, PAGE_SIZE, offset=PAGE_BASE, prot=mmap.PROT_READ)
        val = self.pci_mem_read(mm, offset - PAGE_BASE)
        mm.close()
        os.close(fd)
        return val
    
    def sfp_type(self):
        if self._sfp_type is None and self._xcvr_api is not None:
            id = self._xcvr_api_factory._get_id()
            if id is not None:
                if id in SFP_TYPE_LIST:
                    self._sfp_type = 'SFP'
                elif id in QSFP_TYPE_LIST:
                    self._sfp_type = 'QSFP'
                elif id in QSFP_DD_TYPE_LIST:
                    self._sfp_type = 'QSFP-DD'

        return self._sfp_type

    def _init_sfp(self, delay=False):
        self.init_optoe_driver()
    
    def is_replaceable(self):
        return True

    def get_error_description(self):
        if not self.get_presence():
            return self.SFP_STATUS_UNPLUGGED

        return self.SFP_STATUS_OK

    def get_presence(self):
        """
        Retrieves the presence of the sfp
        Returns: True if sfp is present and False if it is not
        """

        bit_offset = 0
        cpld_offset = 0

        if self._index in range(0, 16):
            bit_offset = self._index
            cpld_offset = 0xf8
        elif self._index in range(16, 32):
            bit_offset = (self._index - 16)
            cpld_offset = 0x188
        elif self._index in range(32, 48):
            bit_offset = (self._index - 16)
            cpld_offset = 0xf8
        elif self._index in range(48, 64):
            bit_offset = (self._index - 32)
            cpld_offset = 0x188
        else:
            bit_offset = self._index - 64

        value = self.pci_get_value(self.BASE_RES_PATH, cpld_offset)

        return (True if (value & (1 << bit_offset)) != 0 else False)

    def get_reset_status(self):
        bit_offset = 0
        cpld_offset = 0

        if self._index in range(0, 16):
            bit_offset = 0
            cpld_offset = 0xf4
        elif self._index in range(16, 32):
            bit_offset = (self._index - 16)
            cpld_offset = 0x184
        elif self._index in range(32, 48):
            bit_offset = (self._index - 16)
            cpld_offset = 0xf4
        elif self._index in range(48, 64):
            bit_offset = (self._index - 32)
            cpld_offset = 0x184
        else:
            bit_offset = self._index - 64

        value = self.pci_get_value(self.BASE_RES_PATH, cpld_offset)

        return (True if (value & (1 << bit_offset)) != 0 else False)

    def init_optoe_driver(self):
        """
        Loads the driver based on the detected SFP
        """
        i2c_path = "/sys/class/i2c-adapter/i2c-1/channel-{0}"
        eeprom_path = "/sys/class/i2c-adapter/i2c-1/channel-{0}/*-0050/eeprom".format(self._index)
        name_path = "/sys/class/i2c-adapter/i2c-1/channel-{0}/*-0050/name".format(self._index)

        port_i2c_path = i2c_path.format(self._index)

        if not os.path.isfile(eeprom_path):
            log_info(f"{eeprom_path} does not exist")
            if self.get_presence():
                log_info(f'Loading optoe3 for port {self._index}')
                if not os.path.isfile(name_path):
                    print("Driver is loaded, deleting it..")
                    try:
                        with open(port_i2c_path + '/delete_device', 'w') as f:
                            f.write('0x50\n')
                    except IOError as e:
                        log_info("Error: Unable to open file: %s, path: %s" % (str(e), port_i2c_path))
                    time.sleep(0.5)
                try:
                    with open(port_i2c_path + '/new_device', 'w') as f:
                        f.write(f'optoe3 0x50\n')
                except IOError as e:
                    if e.errno == 16:
                        time.sleep(0.5)
                    else:
                        log_info("Error: Unable to open file: %s, path: %s" % (str(e), port_i2c_path))
                time.sleep(2)
            else:
                return False
        
        self._xcvr_api = self.get_xcvr_api()
        self.sfp_type()
        self._driver = self.get_optoe_driver()

        try:
            with os.fdopen(os.open(name_path, os.O_RDONLY)) as fd:
                driver_name = fd.read()
                driver_name = driver_name.rstrip('\r\n')
                driver_name = driver_name.lstrip(" ")

            #Avoid re-initialization of the QSFP/SFP optic on QSFP/SFP port.
            if self.sfp_type == 'SFP' and driver_name in ['optoe1', 'optoe3']:
                with open(port_i2c_path + '/delete_device', 'w') as f:
                    f.write('0x50\n')
                time.sleep(0.2)
                with open(port_i2c_path + '/new_device', 'w') as f:
                    f.write('optoe2 0x50\n')
                time.sleep(2)
            elif self.sfp_type == 'QSFP' and driver_name in ['optoe2', 'optoe3']:
                with open(port_i2c_path + '/delete_device', 'w') as f:
                    f.write('0x50\n')
                time.sleep(0.2)
                with open(port_i2c_path + '/new_device', 'w') as f:
                    f.write('optoe1 0x50\n')
                time.sleep(2)
            elif self.sfp_type == 'QSFP_DD' and driver_name in ['optoe1', 'optoe2']:
                with open(port_i2c_path + '/delete_device', 'w') as f:
                    f.write('0x50\n')
                time.sleep(0.2)
                with open(port_i2c_path + '/new_device', 'w') as f:
                    f.write('optoe3 0x50\n')
                time.sleep(2)

        except IOError as e:
            log_info("Error: Unable to open file: %s" % str(e))
            return False
        
        
        
        
        



