use polsys;

function BuffDebuff_AddIcon(character,icone,tempo)
    var packet := CreatePacket(0xDF, MSGLEN_VARIABLE);//0 BYTE[1] Cmd
    //packet.SetInt16(1, packet.GetSize());//1 BYTE[2] Length "tem que ser no fim"
    packet.SetInt32(3, character.serial);//3 BYTE[4] Serial of player
    packet.setint16(7, icone);//7 BYTE[2] Icon Number to show
    packet.setint16(9, 0x1);//9 BYTE[2] 0x1 = Show, 0x0 = Remove. On remove byte, packet ends here.
    packet.SetInt32(11, 0x0);//11 BYTE[4] 0x00000000
    packet.SetInt16(15, icone);//15 BYTE[2] Icon Number to show.
    packet.SetInt16(17, 0x1);//17 BYTE[2] 0x1 = Show
    packet.SetInt32(19, 0x0);//19 BYTE[4] 0x00000000
    packet.SetInt16(23, tempo);//23 BYTE[2] Time in seconds (simple countdown without automatic remove)
    packet.SetInt16(25, 0x0);//25 BYTE[2] 0x0000
    packet.SetInt8(27, 0x0);//27 BYTE[1] 0x00
    packet.SetInt32(28, 1005007);//28 BYTE[4] Cliloc message ID1 cliloc "enu"(cliloc 1042971 == "~1_NOTHING~") (cliloc 1005007 == "")
    packet.SetInt32(32, 1042971);//32 BYTE[4] Cliloc message ID2 cliloc "enu"(cliloc 1042971 == "~1_NOTHING~") (cliloc 1005007 == "")
    packet.SetInt32(36, 0x0);//36 BYTE[4] 0x00000000
    packet.SetInt32(40, 0x1);//40 BYTE[2] 0x0001
    var msg := "<BASEFONT COLOR=#ff00ff>"+icone+"</BASEFONT> ";// tem que ter esse " " para ele concatenar!


    case(icone)
        1001://1001: Dismount
            msg := msg+"<BASEFONT COLOR=#ff00ff>Dismount</BASEFONT>";
        break;
        1002://1002: Disarm
            msg := msg+"<BASEFONT COLOR=#ff00ff>Disarm</BASEFONT>";
        break;
        1005://1005: Nightsight
             msg := msg+"<BASEFONT COLOR=#ff00ff>Nightsight</BASEFONT>";
        break;
        1006://1006: Death Strike
            msg := msg+"<BASEFONT COLOR=#ff00ff>Death Strike</BASEFONT>";
        break;
        1007://1007: Evil Omen
            msg := msg+"<BASEFONT COLOR=#ff00ff>Evil Omen</BASEFONT>";
        break;
        1008://1008: unknown? (GumpID 0x7556)
            msg := msg+"<BASEFONT COLOR=#ff00ff>unknown 1?</BASEFONT>";
        break;
        1009://1009: Regeneration (cliloc1=1044106,cliloc2=1075106)
            msg := msg+"<BASEFONT COLOR=#ff00ff>unknown 2?</BASEFONT>";
        break;
        1010://1010: Divine Fury
            msg := msg+"<BASEFONT COLOR=#ff00ff>Divine Fury</BASEFONT>";
        break;
        1011://1011: Enemy Of One
            msg := msg+"<BASEFONT COLOR=#ff00ff>Enemy Of One</BASEFONT>";
        break;
        1012://1012: Stealth (cliloc1=1044107,cliloc2=1075655)
            msg := msg+"<BASEFONT COLOR=#ff00ff>Stealth</BASEFONT>";
        break;
        1013://1013: Active Meditation (cliloc1=1044106,cliloc2=1075106)
            msg := msg+"<BASEFONT COLOR=#ff00ff>Active Meditation</BASEFONT>";
        break;
        1014://1014: Blood Oath caster
            msg := msg+"<BASEFONT COLOR=#ff00ff>Blood Oath caster</BASEFONT>";
        break;
        1015://1015: Blood Oath curse
            msg := msg+"<BASEFONT COLOR=#ff00ff>Blood Oath curse</BASEFONT>";
        break;
        1016://1016: Corpse Skin
            msg := msg+"<BASEFONT COLOR=#ff00ff>Corpse Skin</BASEFONT>";
        break;
        1017://1017: Mindrot
            msg := msg+"<BASEFONT COLOR=#ff00ff>Mindrot</BASEFONT>";
        break;
        1018://1018: Pain Spike
            msg := msg+"<BASEFONT COLOR=#ff00ff>Pain Spike</BASEFONT>";
        break;
        1019://1019: Strangle
            msg := msg+"<BASEFONT COLOR=#ff00ff>Strangle</BASEFONT>";
        break;
        1020://1020: Gift of Renewal
            msg := msg+"<BASEFONT COLOR=#ff00ff>Gift of Renewal</BASEFONT>";
        break;
        1021://1021: Attune Weapon
            msg := msg+"<BASEFONT COLOR=#ff00ff>Attune Weapon</BASEFONT>";
        break;
        1022://1022: Thunderstorm
            msg := msg+"<BASEFONT COLOR=#ff00ff>Thunderstorm</BASEFONT>";
        break;
        1023://1023: Essence of Wind
            msg := msg+"<BASEFONT COLOR=#ff00ff>Essence of Wind</BASEFONT>";
        break;
        1024://1024: Ethereal Voyage
            msg := msg+"<BASEFONT COLOR=#ff00ff>Ethereal Voyage</BASEFONT>";
        break;
        1025://1025: Gift Of Life
            msg := msg+"<BASEFONT COLOR=#ff00ff>Gift Of Life</BASEFONT>";
        break;
        1026://1026: Arcane Empowerment
            msg := msg+"<BASEFONT COLOR=#ff00ff>Arcane Empowerment</BASEFONT>";
        break;
        1027://1027: Mortal Strike
            msg := msg+"<BASEFONT COLOR=#ff00ff>Mortal Strike</BASEFONT>";
        break;
        1028://1028: Reactive Armor (cliloc1=1075812,cliloc2=1075813)
            msg := msg+"<BASEFONT COLOR=#ff00ff>Reactive Armor</BASEFONT>";
        break;
        1029://1029: Protection (cliloc1=1075814,cliloc2=1075815)
            msg := msg+"<BASEFONT COLOR=#ff00ff>Protection</BASEFONT>";
        break;
        1030://1030: Arch Protection (cliloc1=1075816,cliloc2=1075816)
            msg := msg+"<BASEFONT COLOR=#ff00ff>Arch Protection</BASEFONT>";
        break;
        1031://1031: Magic Reflection (cliloc1=1075817,cliloc2=1075818)
            msg := msg+"<BASEFONT COLOR=#ff00ff>Magic Reflection</BASEFONT>";
        break;
        1032://1032: Incognito (cliloc1=1075819,cliloc2=1075820)
            msg := msg+"<BASEFONT COLOR=#ff00ff>Incognito</BASEFONT>";
        break;
        1033://1033: Disguised
            msg := msg+"<BASEFONT COLOR=#ff00ff>Disguised</BASEFONT>";
        break;
        1034://1034: Animal Form
            msg := msg+"<BASEFONT COLOR=#ff00ff>Animal Form</BASEFONT>";
        break;
        1035://1035: Polymorph (cliloc1=1075824,cliloc2=1075820)
            msg := msg+"<BASEFONT COLOR=#ff00ff>Polymorph</BASEFONT>";
        break;
        1036://1036: Invisibility (cliloc1=1075825,cliloc2=1075826)
            msg := msg+"<BASEFONT COLOR=#ff00ff>Invisibility</BASEFONT>";
        break;
        1037://1037: Paralyze (cliloc1=1075827,cliloc2=1075828)
            msg := msg+"<BASEFONT COLOR=#ff00ff>Paralyze</BASEFONT>";
        break;
        1038://1038: Poison (cliloc1=0x0F8627, cliloc2=0x1069B1)
            msg := msg+"<BASEFONT COLOR=#ff00ff>Poison</BASEFONT>";
        break;
        1039://1039: Bleed (cliloc1=0x106a75,cliloc2=0x106a76)
            msg := msg+"<BASEFONT COLOR=#ff00ff>Bleed</BASEFONT>";
        break;
        1040://1040: Clumsy (cliloc1=0x106a77,cliloc2=0x106a78)
            msg := msg+"<BASEFONT COLOR=#ff00ff>Clumsy</BASEFONT>";
        break;
        1041://1041: Feeble Mind (cliloc1=0x106a79,cliloc2=0x106a7a)
            msg := msg+"<BASEFONT COLOR=#ff00ff>Feeble Mind</BASEFONT>";
        break;
        1042://1042: Weaken (cliloc1=1075837,cliloc2=1075838)
            msg := msg+"<BASEFONT COLOR=#ff00ff>Weaken</BASEFONT>";
        break;
        1043://1043: Curse (cliloc1=1075835,cliloc2=1075836)
            msg := msg+"<BASEFONT COLOR=#ff00ff>Curse</BASEFONT>";
        break;
        1044://1044: Mass Curse (cliloc1=0x106a7f,cliloc2=0x106a80)
            msg := msg+"<BASEFONT COLOR=#ff00ff>Mass</BASEFONT>";
        break;
        1045://1045: Agility (cliloc1=0x106a81,cliloc2=0x106a82)
            msg := msg+"<BASEFONT COLOR=#ff00ff>Agility</BASEFONT>";
        break;
        1046://1046: Cunning (cliloc1=0x106a83,cliloc2=0x106a84)
            msg := msg+"<BASEFONT COLOR=#ff00ff>Cunning</BASEFONT>";
        break;
        1047://1047: Strength (cliloc1=0x106a85,cliloc2=0x106a86)
            msg := msg+"<BASEFONT COLOR=#ff00ff>Strength</BASEFONT>";
        break;
        1048://1048: Bless (cliloc1=0x106a87,cliloc2=0x106a88)
            msg := msg+"<BASEFONT COLOR=#ff00ff>Bless</BASEFONT>";
        break;
        1049://1048: Bless (cliloc1=0x106a87,cliloc2=0x106a88)
            msg := msg+"<BASEFONT COLOR=#ff00ff>ZzZzZ</BASEFONT>";
        break;
        1050://1048: Bless (cliloc1=0x106a87,cliloc2=0x106a88)
            msg := msg+"<BASEFONT COLOR=#ff00ff>homem de pedra </BASEFONT>";
        break;
        1051://1048: Bless (cliloc1=0x106a87,cliloc2=0x106a88)
            msg := msg+"<BASEFONT COLOR=#ff00ff>cavera de fogo</BASEFONT>";
        break;
        1052://1048: Bless (cliloc1=0x106a87,cliloc2=0x106a88)
            msg := msg+"<BASEFONT COLOR=#ff00ff>furia do gargola</BASEFONT>";
        break;
        1054://1048: Bless (cliloc1=0x106a87,cliloc2=0x106a88)
            msg := msg+"<BASEFONT COLOR=#ff00ff>gargola fly</BASEFONT>";
        break;
        default:
            //return 0;
            msg := msg+"<BASEFONT COLOR=#ff00ff>"+icone+"</BASEFONT>";
        break;
    endcase


    packet.SetUnicodeStringFlipped(42,CAscZ(msg),1);//42 BYTE[len(str)*2] Flipped Unicode String (" "+str) (To seperate the entrys add " ")
    //? BYTE[2] 0x0000 , o valor "1" do terceiro parametro da funcao ".SetUnicodeString(,,1)" indica que ele ja coloca um duplo zero no fim.
    packet.SetInt16(1, packet.GetSize());//1 BYTE[2] Length
    packet.sendpacket(character);
    return 1;
endfunction


function BuffDebuff_removeIcon(character,icone)
    var packet := CreatePacket(0xDF, MSGLEN_VARIABLE);//0 BYTE[1] Cmd
    //packet.SetInt16(1, packet.GetSize());//1 BYTE[2] Length "tem que ser no fim"
    packet.SetInt32(3, character.serial);//3 BYTE[4] Serial of player
    packet.setint16(7, icone);//7 BYTE[2] Icon Number to show
    packet.setint16(9, 0x0);//9 BYTE[2] 0x1 = Show, 0x0 = Remove. On remove byte, packet ends here.
    packet.SetInt32(11, 0x0);//11 BYTE[4] 0x00000000
    packet.SetInt16(15, icone);//15 BYTE[2] Icon Number to show.
    packet.SetInt16(17, 0x0);//17 BYTE[2] 0x1 = Show
    packet.SetInt32(19, 0x0);//19 BYTE[4] 0x00000000
    packet.SetInt16(23, 0x0);//23 BYTE[2] Time in seconds (simple countdown without automatic remove)
    packet.SetInt16(25, 0x0);//25 BYTE[2] 0x0000
    packet.SetInt8(27, 0x0);//27 BYTE[1] 0x00
    packet.SetInt32(28, 1005007);//28 BYTE[4] Cliloc message ID1 cliloc "enu"(cliloc 1042971 == "~1_NOTHING~") (cliloc 1005007 == "")
    packet.SetInt32(32, 1005007);//32 BYTE[4] Cliloc message ID2 cliloc "enu"(cliloc 1042971 == "~1_NOTHING~") (cliloc 1005007 == "")
    packet.SetInt32(36, 0x0);//36 BYTE[4] 0x00000000
    packet.SetInt32(40, 0x1);//40 BYTE[2] 0x0001
    var msg := "";// tem que ter esse " " para ele concatenar!
    packet.SetUnicodeStringFlipped(42,CAscZ(msg),1);//42 BYTE[len(str)*2] Flipped Unicode String (" "+str) (To seperate the entrys add " ")
    //? BYTE[2] 0x0000 , o valor "1" do terceiro parametro da funcao ".SetUnicodeString(,,1)" indica que ele ja coloca um duplo zero no fim.
    packet.SetInt16(1, packet.GetSize());//1 BYTE[2] Length
    packet.sendpacket(character);
    return 1;
endfunction