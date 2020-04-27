#from gattlib import DiscoveryService

#service = DiscoveryService("hci0")
#devices = service.discover(2)

#for address, name in devices.items():
    #print("name: {}, address: {}".format(name, address))

from gattlib import GATTRequester

try:
    req = GATTRequester("3C:71:BF:FD:08:96")
    while True:
        data = req.read_by_uuid("00002A37-0000-1000-8000-00805F9B34FB")[0]
        #data = req.read_by_handle(0x180F)[0]
        print("bytes received:", end=' ')
        for b in data:
            print(int(b), end=' ')
        print()

except KeyboardInterrupt:
    exit(1)
