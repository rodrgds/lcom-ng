def scale_xpm(input_filename, output_filename, new_w, new_h):
    old_w = 800
    old_h = 608
    
    # 1. Read the file and automatically filter out the headers/palette
    original_pixels = []
    with open(input_filename, 'r') as f:
        for line in f.readlines():
            if '"' in line:
                parts = line.split('"')
                if len(parts) >= 3:
                    content = parts[1]
                    # Only grab lines that are exactly the original width
                    if len(content) == old_w:
                        original_pixels.append(content)

    if len(original_pixels) != old_h:
        print(f"Error: Found {len(original_pixels)} valid pixel rows instead of {old_h}.")
        print("Make sure map3_raw.txt contains your original 800x608 map!")
        return

    # 2. Open the new file to write the C Array
    with open(output_filename, 'w') as out:
        out.write("/* XPM */\n")
        out.write("static char *smallMap3[] = {\n")
        out.write("/* columns rows colors chars-per-pixel */\n")
        out.write(f"\"{new_w} {new_h} 13 1 \",\n")
        
        # 3. Use an UNUSED character (~) for transparency to fix the duplicate bug!
        palette = [
            "\"~ c None\",\n", 
            "\". c #161C2E\",\n",
            "\"X c #354F59\",\n",
            "\"o c #3B6566\",\n",
            "\"O c #3C7662\",\n",
            "\"+ c #77915C\",\n",
            "\"@ c #839A5E\",\n",
            "\"# c #9BB94E\",\n",
            "\"$ c #80AC5E\",\n",
            "\"% c #B8B958\",\n",
            "\"& c #969F64\",\n",
            "\"* c #47ABA9\",\n",
            "\"= c #C6F0DB\",\n"
        ]
        out.writelines(palette)
        out.write("/* pixels */\n")

        # 4. Downscale and replace spaces
        for y in range(new_h):
            src_y = int(y * old_h / new_h)
            row_chars = []
            
            for x in range(new_w):
                src_x = int(x * old_w / new_w)
                pixel = original_pixels[src_y][src_x]
                
                # Replace transparent spaces with our new safe character
                if pixel == ' ':
                    pixel = '~'
                    
                row_chars.append(pixel)
            
            new_row = "".join(row_chars)
            
            # Format as C string
            if y == new_h - 1:
                out.write(f"\"{new_row}\"\n")
            else:
                out.write(f"\"{new_row}\",\n")
                
        out.write("};\n")
        
    print(f"Success! Scaled map saved to {output_filename}")

if __name__ == "__main__":
    scale_xpm("raw.txt", "smallMap3.h", 395, 300)