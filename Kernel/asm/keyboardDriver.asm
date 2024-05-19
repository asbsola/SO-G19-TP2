GLOBAL get_scan_code

section .text
get_scan_code:
    in al, 0x60
    ret