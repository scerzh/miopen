#!/usr/bin/python3
import re
import sys

arg_pattern = re.compile(r'^([A-Za-z0-9_]+)(?:[ \t]*=[ \t]*([A-Za-z0-9_]+))?$')

def get_arg_name(arg_with_default):
    mtch = arg_pattern.match(arg_with_default)
    if mtch:
        return mtch.groups()[0]
    else:
        print(f"Argument format wasn't understood: {arg_with_default}")
        raise Exception(f"Argument format wasn't understood")

def process_macro(match, prefix):
    macro_indent, macro_name, args, body, endm_indent = match.groups()

    args = args or ""
    if not args.strip():
        return f"{macro_indent}.macro {macro_name}\n{body}{endm_indent}.endm"

    args_with_defaults_list = [arg.strip() for arg in args.split(',')]
    args_list = [get_arg_name(arg) for arg in args_with_defaults_list]
    new_args_list = [f'{prefix}{arg}' for arg in args_list]
    new_args_with_defaults_list = [f'{prefix}{arg}' for arg in args_with_defaults_list]

    new_body = body
    for old_arg, new_arg in zip(args_list, new_args_list):
        new_body = re.sub(rf'\\{old_arg}\b', rf'\\{new_arg}', new_body)

    return f"{macro_indent}.macro {macro_name} {', '.join(new_args_with_defaults_list)}\n{new_body}{endm_indent}.endm"

def add_prefix_to_macros(asm_code, prefix):
    macro_pattern = re.compile(
        r'^([ \t]*)\.macro[ \t]+(\S+)(?:[ \t]+([^\n]*))?\n(.*?)^(\s*)\.endm',
        re.DOTALL | re.MULTILINE
    )
    return macro_pattern.sub(lambda match: process_macro(match, prefix), asm_code)

def main():
    if len(sys.argv) != 3:
        print(f"Usage: python3 {sys.argv[0]} <prefix> <filename>")
        sys.exit(1)

    prefix = sys.argv[1]
    input_filename = sys.argv[2]
    output_filename = f"new_{input_filename}"

    with open(input_filename, "r") as f:
        asm_code = f.read()

    updated_asm_code = add_prefix_to_macros(asm_code, prefix)
    with open(output_filename, "w") as f:
        f.write(updated_asm_code)

    print(f"Result is saved in {output_filename}")

if __name__ == "__main__":
    main()
