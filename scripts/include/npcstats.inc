function npcbonus(who,mount)
    var isMount := GetEquipmentByLayer( who, 25 );

    var str := CInt(GetObjProperty(mount,"strbonus"));
    var dex := CInt(GetObjProperty(mount,"dexbonus"));
    var int := CInt(GetObjProperty(mount,"intbonus"));
    var bonusStat := GetObjProperty(who,"#bonusstats");
    var mounted := GetObjProperty(mount,"mounted");

    if(mounted==1)
        if(!bonusStat)
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
    else
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

    endif

endfunction



function npcbonus22(who,mount)
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