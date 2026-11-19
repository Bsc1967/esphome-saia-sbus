"""Independent reference-vector checks; not a hardware or C++ execution test."""
import binascii

vectors = [
    ("b5000106030000", 0x5F4B),
    ("b5020000", 0x23F4),
    ("b5020001", 0x33D5),
    ("b50100000a9d00000a9d83f30bed1e063dce", 0x78F0),
]
for payload, expected in vectors:
    actual = binascii.crc_hqx(bytes.fromhex(payload), 0)
    assert actual == expected, (payload, actual, expected)
request = bytes.fromhex("b5000106060014")
request += binascii.crc_hqx(request, 0).to_bytes(2, "big")
print("Four independent RACOM CRC vectors passed.")
print("Project request before escaping:", request.hex(" "))
