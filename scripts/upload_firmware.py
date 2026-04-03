import os
import requests
from datetime import datetime

SUPABASE_URL = os.environ['SUPABASE_URL']
SUPABASE_KEY = os.environ['SUPABASE_KEY']

FIRMWARE_PATH = ".pio/build/seeed_xiao_esp32s3/firmware.bin"
VERSION = datetime.now().strftime("v%Y%m%d%H%M")

# Upload firmware to Supabase Storage
with open(FIRMWARE_PATH, 'rb') as f:
    data = f.read()

headers = {
    "Authorization": f"Bearer {SUPABASE_KEY}",
    "Content-Type": "application/octet-stream"
}

upload_url = f"{SUPABASE_URL}/storage/v1/object/firmware/latest.bin"
requests.put(upload_url, data=data, headers=headers)

# Public URL
firmware_url = f"{SUPABASE_URL}/storage/v1/object/public/firmware/latest.bin"

# Save version info (optional, skip DB for now)
print("Uploaded firmware:", firmware_url)
