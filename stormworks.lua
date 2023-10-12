Tickcount = 0
function onTick()
    kmh = input.getNumber(1)
    mph = input.getNumber(2)
    alt = input.getNumber(3)
    bng = input.getNumber(4)
    Tickcount = (Tickcount + 1)%20
    if Tickcount == 0 then
        async.httpGet(8000,string.format("/?kmph=%f&mph=%f&alt=%f&bearing=%f",kmh,mph,alt,bng))
    end    
end