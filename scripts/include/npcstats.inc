function npcbonus(who,mount)
    if(who && mount)
        var bonusStat := GetObjProperty(who,"#bonusstats");
        var str := CInt(GetObjProperty(mount,"strbonus"));
        var dex := CInt(GetObjProperty(mount,"dexbonus"));
        var int := CInt(GetObjProperty(mount,"intbonus"));
        if(bonusStat)
            if(str)
                SetStrengthMod(who, GetStrengthMod(who) - str);
            endif
            if(dex)
                SetDexterityMod(who, GetDexterityMod(who) - dex);
            endif
            if(int)
                SetIntelligenceMod(who, GetIntelligenceMod(who) - int);
            endif
            EraseObjProperty(who, "#bonusstats");
        else
            if(str)
                SetStrengthMod(who, GetStrengthMod(who) + str);
            endif
            if(dex)
                SetDexterityMod(who, GetDexterityMod(who) + dex);
            endif
            if(int)
                SetIntelligenceMod(who, GetIntelligenceMod(who) + int);
            endif
            SetObjProperty(who,"#bonusstats",1);
        endif
    endif
endfunction