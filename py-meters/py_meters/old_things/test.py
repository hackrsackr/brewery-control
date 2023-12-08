import json

from time import sleep

with open('config.json', 'r') as f:
    cfg = json.load(f)
    # print(cfg)

message = cfg["_METERS"]['meter-1']["input-1"]["ads_address"]
print(json.dumps(message, sort_keys=False, indent=4))
