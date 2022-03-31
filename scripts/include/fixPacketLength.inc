use polsys;
use os;
use uo;

function fixPacketLength(packetString, byteLength)
    if ( !packetString || packetString == error )
        packetString := 0;
    endif
    packetString := CStr(packetString);

    if ( packetString[1, 2] == "0x" )
        packetString := CStr(packetString[3, len(packetString)]);
    endif

    if ( len(packetString) > byteLength * 2 )
        //var extraBytes := (byteLength * 2) - len(packetString);
        var extraBytes := len(packetString) - (byteLength * 2) + 1;
        return CStr(packetString[extraBytes, len(packetString)]);
    endif

    while ( len(packetString) < byteLength * 2 )
        packetString := "0" + packetString;
    endwhile
    return packetString;

endfunction