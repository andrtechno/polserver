
function ExceptionalFameMod(who, skill, diff, points)
  if(skill > diff - 20)
    diff := diff + 20;
    if((skill - diff) <= 5)
      points := points;
    elseif((skill - diff) <= 10)
      points := (points * 3) / 4;
    elseif((skill - diff) <= 15)
      points := points / 2;
    elseif((skill - diff) <= 20)
      points := points / 4;
    else
      points := 0;
    endif
    points := Cint(points);
    var fame := Cint(GetObjProperty(who, "Fame"));
    var famegain := fame + points;
    SetObjProperty(who, "Fame", famegain);
    var msgtext := "You have ";
    if(points < 0)
      msgtext := msgtext + "lost ";
      points := Abs(points);
    else
      msgtext := msgtext + "gained ";
    endif
    if(points > 150)
      msgtext := msgtext + "a great amount of ";
    elseif(points > 125)
      msgtext := msgtext + "alot of ";
    elseif(points > 75)
      msgtext := msgtext + "a good amount of ";
    elseif(points > 30)
      msgtext := msgtext + "some ";
    elseif(points > 0)
      msgtext := msgtext + "a little ";
    else
      return 0;
    endif
    msgtext := msgtext + "Fame.";
    SendSysMessage(who, msgtext);
    var karma := Cint(GetObjProperty(who, "Karma"));
    var kmod := GetKarmaLevel(karma);
    var fmod := GetFameLevel(famegain);
    var newtitle := nototitles[ (kmod) ];
    newtitle := "The " + CStr(newtitle[fmod]) + " ";
    if(newtitle["None"])
      newtitle := "";
    endif
    if(fmod == 5)
      if(who.gender == 1)
        newtitle := newtitle + "Lady ";
      else
        newtitle := newtitle + "Lord ";
      endif
    endif
    if(newtitle != who.title_prefix)
      who.title_prefix := newtitle;
      SendSysMessage(who, "you are now known as " + newtitle + who.name);
    endif
    SetNotoTitle(who, karma, fame);
  endif
endfunction


function setquality(who, bow)
  var tname := TruncateArticle(bow.name);
  if(CInt(AP_GetSkill(who, BOWCRAFT)) >= 99)
    bow.name := "an exceptional " + tname + " [crafted by " + who.name + "]";
  else
    bow.name := "an exceptional " + tname;
  endif
  bow.quality := bow.quality + 0.2;
  bow.hp := bow.maxhp;
  SendSysMessage(who, "You created an exceptional item.");
endfunction