import cantools
import sys
from typing import List

def parse_dbc(filename: str):
    with open(filename, "r") as f:
        db = cantools.database.load(f)
    return db

def generate_c_code(db):
    c_code = """#ifndef __CAN_CONFIG__
#define __CAN_CONFIG__
#include \"CAN_Signal.hpp\"
#include \"etl/map.h\"
#include \"etl/vector.h\"
#include \"etl/set.h\"

// Defining the CAN_IDs & Signals
"""

    message_definitions = []
    signal_definitions = []
    can_map_entries = []
    
    for message in db.messages:
        if "MOBO" in message.senders or "INV" in message.senders or "VCU" in message.senders: 
            message_name = message.name.upper()
            message_id = message.frame_id
            message_definitions.append(f"#define {message_name} {message_id}")
            
            signal_list = []
            for signal in message.signals:
                signal_name = f"{signal.name}_ID{message.frame_id}"
                is_intel = "false" if signal.byte_order == "big_endian" else "true"
                offset = signal.start
                length = signal.length
                scale = f", {signal.scale}f" if signal.scale != 1 else ""
                
                signal_def = f"inline CAN_Signal {signal_name}{{{is_intel}, {offset}, {length}{scale}}};"
                signal_definitions.append(signal_def)
                signal_list.append(f"&{signal_name}")
            
            signals_vector = ",\n        ".join(signal_list)
            can_map_entries.append(f"    etl::pair{{{message_name}, etl::vector<CAN_Signal*, 8>{{ \n        {signals_vector} \n        }} }}")
    
    c_code += "\n".join(message_definitions) + "\n\n"
    c_code += "\n".join(signal_definitions) + "\n\n"
    c_code += "// Define the CAN Map\n"
    c_code += "inline etl::map CAN_Map\n{ \n" + ",\n".join(can_map_entries) + "\n};\n"
    c_code += "inline etl::set CAN_Rx_IDs = {"
    for message in db.messages:
        if "VCU" not in message.senders: 
            c_code += f"{message.frame_id},"    
    c_code += "};"
    # c_code += "\n#endif // __CAN_CONFIG__\n"
    
    return c_code

def main(dbc_file: str, output_file: str):
    db = parse_dbc(dbc_file)
    c_code = generate_c_code(db)
    with open(output_file, "w") as f:
        f.write(c_code)
    print(f"Generated C file: {output_file}")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python script.py input.dbc output.h")
        sys.exit(1)
    
    main(sys.argv[1], sys.argv[2])