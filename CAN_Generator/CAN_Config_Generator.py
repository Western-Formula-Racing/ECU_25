import cantools
import sys

def parse_dbc(filename):
    """Parses a DBC file and extracts CAN message and signal definitions."""
    db = cantools.database.load_file(filename)
    
    messages = []
    for message in db.messages:
        signals = []
        for signal in message.signals:
            print(f"contents of signal from dbc: {signal}")
            scale = signal.scale if signal.scale != 1 else None
            signals.append({
                "name": signal.name,
                "start": signal.start,
                "length": signal.length,
                "is_float": signal.is_float,
                "scale": scale
            })
        messages.append({
            "name": message.name,
            "id": message.frame_id,
            "signals": signals
        })
    
    return messages

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python generate_can_code.py <DBC_FILE>")
        sys.exit(1)
    
    dbc_file = sys.argv[1]
    messages = parse_dbc(dbc_file)
    print(f"messages: {messages}")
    # c_code = generate_c_code(messages)
    
    # with open("generated_can.h", "w") as f:
    #     f.write(c_code)
    
    # print("C code successfully generated in 'generated_can.h'")
