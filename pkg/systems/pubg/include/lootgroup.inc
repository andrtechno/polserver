

function CreateFromStackString(who, elemdb, str)
    var params := SplitWords(str);
    var count;
    var objname;
    var color;
    var chance;
    count := RandomDiceRoll(CStr(params[1]));
    params[1] := params[2];
    params[2] := params[3];
    params[3] := params[4];
    if (!count)
        count := 1;
    endif
    objname := params[1];
    objname := Lower(objname);
    if(CInt(params[2]))
        color := CInt(params[2]);
    else
        color := 0;
    endif
    if (CInt(params[3]))
        chance := CInt(params[3]);
    else
        chance := 100;
    endif
    var rand := RandomInt(100)+1;
    if(CInt(rand) <= CInt(chance))
        if (CInt(objname))
            objname := CInt(objname);
        else
            Lower(objname);
        endif
        var item := CreateItemInContainer(who, objname, count);
        if(!item)
            syslog("Unable to create " + objname + " on " + who.name + ". Error= " + item.errortext);
        endif
        item.color := color;

        foreach itemb in EnumerateItemsInContainer(who)
            itemb.decayat:=ReadGameClock() + 30;
            itemb.name:="TEST2";
        endforeach

    //if(making_starting_equipment)
    //  item.newbie := 1;
    //endif
    endif
endfunction

function CreateFromItemString(who, elemdb, str)
  var params := SplitWords(str);
  var count, objname, color, chance;
  var couleurs := ReadConfigFile("::colors");
  count := RandomDiceRoll(CStr(params[1]));
  params[1] := params[2];
  params[2] := params[3];
  params[3] := params[4];
  if (!count)
    count := 1;
  endif
  objname := params[1];
  if(CInt(params[2]))
    color := CInt(params[2]);
  elseif (params[2])
    color := 0;
    var colorg := FindConfigElem(couleurs, params[2]);
    if(colorg)
      var str := GetConfigStringArray(colorg, "color");
	  var ar := {};
	  var rndint := RandomInt(len(str)) + 1;
	  ar := SplitWords(str[rndint]);
	  if(len(ar) > 1)
	    if(CInt(ar[2]) > 0)
	      color := RandomInt(CInt(ar[2]) - CInt(ar[1]))+1;
	    else
	      color := CInt(str[RandomInt(len(str))+1]);
	    endif
	  else
            color := CInt(str[RandomInt(len(str))+1]);
	  endif
	  if(color == 1)
	    color := 0;
	  endif
    endif
  endif
  if (!color)
    color := 0;
  endif
  if(CInt(params[3]))
    chance := CInt(params[3]);
  else
    chance := 100;
  endif
  if(CInt(RandomInt(100)+1) <= CInt(chance))
    var i;
    for(i:=1; i<=CInt(count); i:=i+1)
      if (CInt(objname))
        objname := CInt(objname);
      else
        Lower(objname);
      endif
      var item := CreateItemInContainer(who, objname);
        if(!item)
          syslog("Unable to create " + objname + " on " + who.name + ". Error= " + item.errortext);
        endif
      item.color := color;

    var itemList := array;
    foreach itemb in EnumerateItemsInContainer(who)
	   itemb.decayat := ReadGameClock() + 30;
	   itemb.name := "TEST";
        itemList.append(itemb.serial);
    endforeach
    elemdb.SetProp("items", itemList);

    endfor
  endif
endfunction


function CreateFromGroup(who, elemdb, group)

  //if(making_starting_equipment != 1)

 // endif
  var elem := qz_loot[group];
  foreach str in GetConfigStringArray(elem, "Stack")
    CreateFromStackString(who, elemdb, str);
  endforeach
  foreach str in GetConfigStringArray(elem, "Item")
    CreateFromItemString(who, elemdb, str);
  endforeach
  foreach str in GetConfigStringArray(elem, "Random")
    CreateFromRandomString(who, elemdb, str);
  endforeach
  foreach str in GetConfigStringArray(elem, "Group")
    CreateFromGroup(who, elemdb, str);
  endforeach
  //foreach str in GetConfigStringArray(elem, "Unique")
  //  CreateFromUniqueString(who, elemdb, str);
  //endforeach
endfunction



function CreateFromRandomString(who, elemdb, desc)

  var params := SplitWords(desc);
  var amt := Cint(params[1]);
  var count := 1;
  if(amt != 1)
    count := RandomDiceRoll(Cstr(amt + 1));
  endif
  var group := params[2];
  var chance := params[4];
  var elem := FindConfigElem(qz_loot, group);
  if(elem == error)
    syslog("Unable to find group " + group);
    return;
  endif
  var items := GetConfigStringArray(elem, "Item");
  if(items == error)
    syslog("Group " + group + " does not contain items!");
    return;
  endif
  if(!chance)
    chance := 100;
  endif
  var rand := RandomInt(99)+1;
  if(CInt(rand) <= CInt(chance))
    if(count > len(items))
      count := len(items);
    endif
    while(count)
      var n := RandomInt(len(items)) + 1;
      if(items[n])
        CreateFromItemString(who, elemdb, items[n]);
        count := count - 1;
      endif
    endwhile
  endif
endfunction

