message = "GUR HFREANZR VF: UHRUHRUHR GUR ONPXHC CNFFJBEQ VF: V NZ GUR ORFG QBTR"
alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

#message and alphabet for the weird alphabet
#message = 'SGFBOYXYCVAMYYSVAFMRZDVTAMOSOHOYFCFAFRZBIORZSFQSOZMUVXBIFSFIDKTIZFAOHIFOYYTUGRAFBIOUFMOIXISGFYSVAFMBOYXYCVAMYCRSGSGFRAUVAAFYBVZMRZEAVIXIRZEGOYGFYTYRZEVZFVLSGFBARXFYOYXYFKFZSFXFZOZMSGFVSGFAOYVZFFQBZRZFBITYXYFKFZDVTYGVTIMHFOHIFSVOUUFYXYSGFCFHYRSFRLDVTUOZEFZFAOSFSGFUVAXAFYBVZMRZEGOVW'
#alphabet = 'FGBQMYTCZKORLEHPXDSUWNVAI'


for key in range(0,len(alphabet)):
    ans = ""
    for c in message:
        
        num = alphabet.find(c)
        num = num - key
        
        if num<0:
            num += len(alphabet)
        ans = ans + alphabet[num]

    #print("Shift value :%d and cleartext :%s",key,ans)
    print("Shift value is : "+str(key)+" and cleartext is :"+ans)
        
        

   
