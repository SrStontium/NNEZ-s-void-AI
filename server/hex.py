def hex_to_text(hex_string):
    byte_string = bytes.fromhex(hex_string)
    text = byte_string.decode('utf-8')

    return text


def text_to_hex(text):
    byte_string = text.encode('utf-8')
    hex_string = byte_string.hex()
    return hex_string
