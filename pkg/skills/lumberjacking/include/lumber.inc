function GenerateNewWoodType()

    var val := RandomInt(1499) + 1;


    if((val >= 1) && (val <= 400)) //Jake
        return 2;
    elseif((val >= 401) && (val <= 500)) //Oak
        return 3;
    elseif((val >= 501) && (val <= 600)) //Karund
        return 4;
    elseif((val >= 601) && (val <= 700)) //Leshram
        return 5;
    elseif((val >= 701) && (val <= 800)) //Turmalit
        return 6;
    elseif((val >= 801) && (val <= 900)) //Emerint
        return 7;
    elseif((val >= 901) && (val <= 1000)) //Legrand
        return 8;
    elseif((val >= 1001) && (val <= 1100)) //Solmur
        return 9;
    elseif((val >= 1101) && (val <= 1150)) //Kleor
        return 10;
    elseif((val >= 1151) && (val <= 1200)) //Lorgadum
        return 11;
    elseif((val >= 1201) && (val <= 1250)) //Vialonit
        return 12;
    elseif((val >= 1251) && (val <= 1300)) //Stardust
        return 13;
    elseif((val >= 1301) && (val <= 1350)) //Pironil
        return 14;
    elseif((val >= 1351) && (val <= 1400)) //Mistyc
        return 15;
    elseif((val >= 1401) && (val <= 1450)) //Elvin
        return 16;
    elseif((val >= 1451) && (val <= 1499)) //Elkris
        return 17;
    endif
return 1;
endfunction







//skill := AP_GetSkill( who, LUMBERJACKING);
function GetWoodTypeBySkill(skill)

    if(skill >= 145)
        return 17;
    elseif(skill >= 140)
        return 16;
    elseif(skill >= 135)
        return 15;
    elseif(skill >= 128)
        return 14;
    elseif(skill >= 120)
        return 13;
    elseif(skill >= 115)
        return 12;
    elseif(skill >= 110)
        return 11;
    elseif(skill >= 105)
        return 10;
    elseif(skill >= 100)
        return 9;
    elseif(skill >= 95)
        return 8;
    elseif(skill >= 90)
        return 7;
    elseif(skill >= 85)
        return 6;
    elseif(skill >= 80)
        return 5;
    elseif(skill >= 75)
        return 4;
    elseif(skill >= 70)
        return 3;
    elseif(skill >= 60)
        return 2;
    endif
return 1;

endfunction