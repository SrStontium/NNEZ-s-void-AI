def hex_to_text(hex_string):
    """
    Converts a hexadecimal string to its corresponding UTF-8 text.

    Args:
        hex_string (str): A hexadecimal string representing the encoded text.

    Returns:
        str: The decoded text in UTF-8 encoding.

    Raises:
        ValueError: If the input is not a valid hexadecimal string.
    """
    try:
        # Convert the hexadecimal string to a byte string
        byte_string = bytes.fromhex(hex_string)
        # Decode the byte string into a UTF-8 text
        text = byte_string.decode('utf-8')
        return text
    except ValueError as e:
        raise ValueError(f"Invalid hexadecimal input: {hex_string}") from e


def text_to_hex(text):
    """
    Converts a UTF-8 text to its hexadecimal representation.

    Args:
        text (str): The text to be converted.

    Returns:
        str: The hexadecimal representation of the text.
    """
    # Encode the text into a byte string
    byte_string = text.encode('utf-8')
    # Convert the byte string to a hexadecimal string
    hex_string = byte_string.hex()
    return hex_string


print(hex_to_text("e69d8e"))
