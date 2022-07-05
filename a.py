a = input().replace("\"", "").split(".")

str_number = str(int(''.join(a[1:]), base=2))

exp = int(a[0][8:17], base=2)
print(f"{'-' if a[0][0] == '1' else ''}{str_number[:28 - exp] + '.' + str_number[28 - exp:] if exp else str_number}")