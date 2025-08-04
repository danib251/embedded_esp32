# convert_files_to_header.py
import os

files = ["index.html", "style.css", "script.js"]
output_header = "../src/web_files.h"

def to_c_array(varname, data):
    lines = []
    lines.append(f"unsigned char {varname}[] = {{")
    for i in range(0, len(data), 12):
        chunk = data[i:i+12]
        hex_chunk = ", ".join(f"0x{b:02x}" for b in chunk)
        lines.append(f"  {hex_chunk},")
    lines.append("};")
    lines.append(f"unsigned int {varname}_len = {len(data)};\n")
    return "\n".join(lines)

with open(output_header, "w", encoding="utf-8") as out:
    for filename in files:
        varname = filename.replace(".", "_").replace("-", "_")
        if not os.path.exists(filename):
            print(f"❌ Archivo no encontrado: {filename}")
            continue
        with open(filename, "rb") as f:
            data = f.read()
            c_array = to_c_array(varname, data)
            out.write(c_array + "\n")
print(f"✅ Archivo generado: {output_header}")
