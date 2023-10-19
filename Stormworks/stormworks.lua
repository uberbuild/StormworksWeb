Tickcount = 0
function onTick()
    kmh = input.getNumber(1)
    mph = input.getNumber(2)
    alt = input.getNumber(3)
    bng = input.getNumber(4)
    pch = input.getNumber(5)
    rll = input.getNumber(6)
    up_ = input.getNumber(7)
    Tickcount = (Tickcount + 1)%20
    if Tickcount == 0 then
        async.httpGet(8000,string.format("/?kmph=%f&mph=%f&alt=%f&bearing=%f&pitch=%f&roll=%f&up=%f",kmh,mph,alt,bng,pch,rll,up_))
    end
end