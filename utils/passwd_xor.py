import operator

"""
passwd_xor function
Antoine LANDRIEUX MIT License

This code is the Borium login encryption algorithm.
 -> Not yet implemented
"""
def passwd_xor() -> str:
    raw: str = input(" PassWd       : ")
    ret: str = ""
    key: int = 1

    for c in raw:
        key += key + ord(c)
        key = key % 100
        ret += chr(operator.xor(key, ord(c)))

    items = []
    for item in ret:
        items.append(item.encode("ascii").hex())
    print(" PassWdXorHex : " + "".join(f"{str(item).upper()} " for item in items))

    return ret


if __name__ == "__main__":
    passwdxor: str = passwd_xor()
    print("******[DRAW]******")
    print(passwdxor)
    print("******************")
