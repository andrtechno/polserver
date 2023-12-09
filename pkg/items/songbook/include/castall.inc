use uo;
use os;

include ":timedScripts:timedScripts";

function CastAll(me, song, duration, points)

    var party := GetObjProperty(me, "#Party");

	if ( GetObjProperty(me, song) )
        SendSysMessage(me, "Already under the influence!");
    else
        TS_StartTimer(me, song, duration, points, me);
	endif



    //Guilds
    foreach mob in ListMobilesNearLocation(me.x, me.y, LIST_IGNORE_Z, 15, me.realm)
        if(me.guildid)
            if(mob.acctname && (mob.guildid == me.guildid) && (mob.serial != me.serial))
                if (!CInt(GetObjProperty(mob, song)))
                    TS_StartTimer(mob, song, duration, points, me);
                endif
            endif
        endif
    endforeach


    //Party
    foreach prt in party
        var enemy := SystemFindObjectBySerial(prt);
        if( Distance( me, enemy ) < 15 && !GetObjProperty(enemy, song) && (enemy.serial != me.serial))
            TS_StartTimer(enemy, song, duration, points, me);
        endif
    endforeach

    return 1;
endfunction

