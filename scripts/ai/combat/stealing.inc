
function ThiefStealing(opponent)
    if((opponent.cmdlevel > 0) || (opponent.serial == me.serial))
        //SetWarMode(0);
        //opponent := 0;
        //return;
    endif
    set_priority(50);
    TurnToward(opponent);
    var loops := 0;
    var f_ev;
    var fwaittime := 0;
var anch := GetObjProperty(me, "Anchor");
    while((opponent) && (!opponent.dead) && (!opponent.hidden) && (!opponent.concealed) && (Distance(me,opponent) < 20))
        if(!CloseDistance(opponent))
            loops := loops + 1;
            fwaittime := 0;
        else
            loops := 0;
            fwaittime := 1;
        endif
        me.stealthsteps := 1000;
        f_ev := wait_for_event(fwaittime);
        //process_combat_event(opponent);
        //if(f_ev.type != SYSEVENT_SPEECH)
            //case (f_ev.type)
            //SYSEVENT_ENTEREDAREA:
               //opponent := f_ev.source;


WalkToward(opponent);
                //if((f_ev.source.npctemplate) and (CheckLineOfSight(me, opponent)))
                //    WalkToward(opponent);
                if((Distance(me, opponent) <= 2))

                var list := array;
                foreach item in (EnumerateItemsInContainer(opponent.backpack, ENUMERATE_ROOT_ONLY))
                    if(!item.newbie && !item.isa(POLCLASS_CONTAINER))
                        //print(item.name);
                        list.Append(item.serial);
                    endif
                endforeach
                sleep(3);

                //stealing 50% success
                //if(RandomInt(99)+1 >= 50)
                    if(list.Size())
                        var rint := RandomInt(list.Size())+1;
                        var itemLoot := SystemFindObjectBySerial(list[rint]);
                        print("Спиздил: "+itemLoot.name);
                        MoveItemToContainer(itemLoot, me.backpack);
                        list.Erase(rint);
                        //me.criminal := 1;
                    else
                        print("все лох обработан, валим...");
                        GoAnchor(anch);
                    endif
                //endif
                sleep(3);
          endif;

            //endcase
        //endif
    endwhile
    //post_combat();
endfunction

function EnableCombatEvents()
    EnableEvents(SYSEVENT_ENGAGED + SYSEVENT_DAMAGED + EVID_PEACEMADE);
    DisableEvents(SYSEVENT_SPEECH);
endfunction

function DisableCombatEvents()
    DisableEvents(SYSEVENT_ENGAGED + SYSEVENT_DAMAGED + EVID_PEACEMADE);
endfunction

function flee(opponent)
  if((me.script == "immobile") || (me.script == "immobilespell"))
	sleep(2);
	return;
  else
    var numflees := GetObjProperty(me,"#flees");
	if(numflees > 10)
      RestartScript(me);
	else
	  numflees := numflees + 1;
	endif
    SetObjProperty(me,"#flees", numflees);
    var runs := 0;
    var chk := 0;
    var f_f_ev;
    while((Distance(me, opponent) < 15) && (runs < 50) && (CheckLineOfSight(me, opponent)))
      f_f_ev := wait_for_event(1);
      chk := process_flee_event(opponent);
	  WalkAwayFrom(opponent);
	  runs := runs +1;
	  case (f_f_ev.type)
        SYSEVENT_DAMAGED: if(((f_f_ev.source.npctemplate) and (CheckLineOfSight(me, f_f_ev.source))) ||(Distance(me, f_f_ev.source) < Distance(me, opponent)) || (!CheckLineOfSight(me, opponent)) || (!opponent) || (RandomInt(3)==1))
					        opponent := f_f_ev.source;
			  		        SetOpponent(opponent);
			              endif
        EVID_PEACEMADE:   chk := 1;
        EVID_HERDING:     Herd(f_f_ev);
	  endcase
	  if(chk)
	    break;
	  endif
    endwhile
  endif
endfunction

function GoAnchor(anch)

    var i := 0;

    while(1)
        EraseObjProperty(me, "opponent");
        WalkTowardLocation(CInt(anch[1]),CInt(anch[2]));

        sleepms(20);

        if(CoordinateDistance(me.x, me.y, anch[1], anch[2]) <= 0)
            RestartScript(me);
            break;
        endif
        i++;
    endwhile
endfunction
