include "ai/setup/main_ai";

function main_AI_loop()
    var ev := os::wait_for_event(0);
    var wanders := 60;
    var next_loot := ReadGameClock() + 20;
    EnableMainEvents();
    var rint := 0;

    while(1)
        look_around();
        wander();
        if(ReadGameClock() > next_loot)
            grabloot();
            next_loot := ReadGameClock() + 10;
        endif
        rint := RandomInt(4) + 1;
        if(rint == 4)
            PlaySoundEffect(me, idlesnd1);
        elseif(rint == 3)
            PlaySoundEffect(me, idlesnd2);
        endif
        wanders := wanders +1;
        if(wanders >= 60)
            wanders := 0;
            ev := sleepmode();
        else
            ev := os::wait_for_event(5);
        endif
        if(ev)
            case(ev.type)
                SYSEVENT_ENTEREDAREA:
//CheckLineOfSight( me, ev.source ) &&
if(((!ev.source.npctemplate) or (ev.source.script == "employed") || (ev.source.script == "tamed")))

//var pather := FindPath( 1436, 1636, 20, 1426,1648,10, me.realm, flags := FP_IGNORE_MOBILES, searchskirt := 500, movemode := "L" );
//print(pather);
//foreach p in (pather)
//print("pather2");
//RunTowardLocation(p.x, p.y);
//sleepms(100);
//endforeach;
//endif;


                        Fight(ev.source);
print("SYSEVENT_ENTEREDAREA 1");
/*
var desc := struct{
Color    := 0,
Graphic  := 0x122d,
Movable  := 1,
ObjClass := "Item",
ObjType  := 0x122d
};
var item := CreateItemAtLocation(
ev.source.x+ConvertDirectionToLocX(ev.source.facing),
ev.source.y+ConvertDirectionToLocY(ev.source.facing),
ev.source.z, desc, 1, ev.source.realm
);*/


                    endif
                SYSEVENT_ENGAGED:       Fight(ev.source); print("SYSEVENT_ENGAGED 1");
                EVID_ALL_ATTACK_CMD:    Fight(ev.source); print("EVID_ALL_ATTACK_CMD 1");
                SYSEVENT_DAMAGED:       Fight(ev.source); print("SYSEVENT_DAMAGED 1");
            endcase
        endif
    endwhile
endfunction

function look_around()
    Say("look_around");
    if(AP_GetVital(me, VITALID_LIFE) < AP_GetVitalMaximumValue(me, VITALID_LIFE))
        CastSpell(me, me, "greaterHeal");
        print("look_around heal");
    endif


    var anch := GetObjProperty(me, "Anchor");
    if(CoordinateDistance(me.x, me.y, anch[1], anch[2]) > HALT_THRESHOLD)
        WalkTowardLocation( CInt(anch[1]), CInt(anch[2]) );
    endif

    foreach npc in ListMobilesInLineOfSight(me, HALT_THRESHOLD)

        if(npc.cmdlevel <= 0)
            if((!npc.npctemplate) || (npc.script == "employed") || (npc.script == "tamed"))
                Say("look_around "+npc.name);
                Fight(npc);
            endif
        endif
    endforeach
endfunction

function EnableMainEvents()
    DisableEvents(SYSEVENT_SPEECH + SYSEVENT_LEFTAREA + SYSEVENT_DISENGAGED + SYSEVENT_OPPONENT_MOVED);
    EnableEvents(SYSEVENT_ENGAGED + SYSEVENT_DAMAGED + SYSEVENT_ENTEREDAREA, HALT_THRESHOLD);
endfunction

function DisableMainEvents()
    DisableEvents(SYSEVENT_ENGAGED + SYSEVENT_DAMAGED + SYSEVENT_ENTEREDAREA);
endfunction


function CloseIn(me, lx, ly)
  case (CoordinateDistance(me.x, me.y, lx, ly))
    0:       return 1;
    default: WalkTowardLocation(lx, ly);
             return 0;
  endcase
endfunction
