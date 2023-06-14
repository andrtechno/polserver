
function Fight(opponent)
    if((opponent.cmdlevel > 0) || (opponent.serial == me.serial))
        SetWarMode(0);
        opponent := 0;
        return;
    endif
    /*var oldprio :=*/ set_priority(50); // 'oldprio' unnecessary unless we're going to restore it
    SetOpponent(opponent);
    prepare_for_fight(opponent);
    TurnToward(opponent);

    SetObjProperty(me, "opponent", opponent.serial);
    var loops := 0;
    var f_ev;
    var fwaittime := 0;

    SetObjProperty(me, "time_attack", ReadGameClock() + 30); // && (opponent.cmdlevel<1)
    while((opponent) && (!opponent.dead) && (opponent.cmdlevel<1) && (!opponent.hidden) && (!opponent.concealed) && (Distance(me,opponent) < 20))
        if(!CloseDistance(opponent))
            loops := loops + 1;
            fwaittime := 0;
        else
            loops := 0;
            fwaittime := 1;
        endif
        f_ev := wait_for_event(fwaittime);
	    process_combat_event(opponent);

        var anch := GetObjProperty(me,"Anchor");


        if(CoordinateDistance(me.x, me.y, CInt(anch[1]), CInt(anch[2])) > 15)
                //MoveObjectToLocation(me, anch[1], anch[2], anch[3],me.realm, MOVEOBJECT_FORCELOCATION);
            //TurnTowardLocation( anch[1], anch[2] );
            //Detach();
            GoAnchor(anch);

        endif


        var mobiles := ListMobilesInLineOfSight(me, 15);
        var playersList := {};
        if((len(mobiles) > 1))
            foreach player in mobiles
                if(player.acctname)
                    playersList.append(player);
                endif
            endforeach
        endif



        if(ReadGameClock() > CInt(GetObjProperty(me, "time_attack")))
            opponent := playersList[RandomInt(playersList.Size()) + 1];
            SetOpponent(playersList[RandomInt(playersList.Size()) + 1]);
            TurnToward(opponent);
            SetObjProperty(me, "opponent",opponent.serial);
            SayUC("А ну иди сюда! "+opponent.name, SAY_TEXTTYPE_DEFAULT, "RUS", SAY_DOEVENT_ENABLE );
            return 0;
        endif



        if(f_ev.type != SYSEVENT_SPEECH)
            case (f_ev.type)
                SYSEVENT_DAMAGED:
                    if((f_ev.source.npctemplate) and (CheckLineOfSight(me, f_ev.source)))
                        opponent := f_ev.source;
                        SetOpponent(opponent);
                        TurnToward(opponent);
                        print("SYSEVENT_DAMAGED 2");
                    elseif((Distance(me, f_ev.source) < Distance(me, opponent)) || (!CheckLineOfSight(me, opponent)) || (!opponent))
                        opponent := f_ev.source;
                        SetOpponent(opponent);
                        TurnToward(opponent);
                    elseif(RandomInt(3)==1)
                        opponent := f_ev.source;
                        SetOpponent(opponent);
                        TurnToward(opponent);
                    endif
            SYSEVENT_ENGAGED:
                if(f_ev.source)
                    if(RandomInt(6) == 1)
                        opponent := f_ev.source;
                        SetOpponent(opponent);
                        TurnToward(opponent);
                    endif
                endif

            endcase
        endif
    endwhile
    post_combat();
endfunction

function EnableCombatEvents()
    EnableEvents(SYSEVENT_ENGAGED + SYSEVENT_DAMAGED );
    DisableEvents(SYSEVENT_SPEECH);
endfunction

function DisableCombatEvents()
    DisableEvents(SYSEVENT_ENGAGED + SYSEVENT_DAMAGED);
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
	  endcase
	  if(chk)
	    break;
	  endif
    endwhile
  endif
endfunction




function GoAnchor(anch)

    while(1)
        RunTowardLocation(CInt(anch[1]),CInt(anch[2]));
        print("while GoAnchor "+CoordinateDistance(me.x, me.y, anch[1], anch[2]));

        if(CoordinateDistance(me.x, me.y, anch[1], anch[2]) <= 0)
            RestartScript(me);
            break;
        endif
    endwhile

endfunction

