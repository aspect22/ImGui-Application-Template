import sys

def png_to_c_array(input_path, output_path, var_name):
    with open(input_path, 'rb') as f:
        data = f.read()

    hex_bytes = [f"0x{b:02X}" for b in data]
    line_len = 12
    lines = [
        ', '.join(hex_bytes[i:i + line_len])
        for i in range(0, len(hex_bytes), line_len)
    ]
    
    with open(output_path, 'w') as out:
        out.write('#pragma once\n')
        out.write(f'unsigned char {var_name}[] = {{\n')
        for line in lines:
            out.write(f'    {line},\n')
        out.write('};\n')
        out.write(f'int {var_name}_len = {len(data)};\n')

# Example usage:
# png_to_c_array("logo.png", "logo.h", "logo_p")

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: python png_to_c_array.py <input.png> <output.h> <variable_name>")
        sys.exit(1)
    png_to_c_array(sys.argv[1], sys.argv[2], sys.argv[3])
