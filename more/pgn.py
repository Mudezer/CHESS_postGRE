import re

def remove_brackets(text):
    # Regular expression pattern to match square or curly brackets and their contents
    
    # Remove the matched patterns from the text
    cleaned_text = re.sub(r"\[[^\]]*\]|\{[^}]*\} ", "", text)
    cleaned_text2 = re.sub(r"\d+\.\.\. ", "", cleaned_text)
    
    return ("('" + cleaned_text2[:-1] + "'),\n")

# Define the input and output file paths
input_file = "OnYx511pgn.txt"
output_file = "OnYx511pgn_san.txt"

# Open the input file for reading
with open(input_file, "r") as file:
    # Read the lines from the input file
    lines = file.readlines()

# Open the output file for writing
with open(output_file, "w") as file:
    # Iterate over the lines
    for line in lines:
        new_line = remove_brackets(line)
        file.write(new_line)

print("Lines modified and written to the output file.")
