s = "I AM THE BEST DOGE"
p = 17
m = (10**9) + 7
hash_value  = 0
p_pow = 1
for c in s:
    hash_value = (hash_value + (ord(c))*p_pow)%m
    p_pow = (p_pow*p)%m

print(hash_value)