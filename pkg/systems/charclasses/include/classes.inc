use uo;

include "include/client";
include ":attributes:caps";

//this deperecate
const CLASSEID_BARD       := "IsBard";
const CLASSEID_CRAFTER    := "IsCrafter";
const CLASSEID_MAGE       := "IsMage";
const CLASSEID_RANGER     := "IsRanger";
const CLASSEID_THIEF      := "IsThief";
const CLASSEID_WARRIOR    := "IsWarrior";



const ID_MAGE       := 1;
const ID_WARRIOR    := 2;
const ID_CRAFTER    := 3;
const ID_RANGER     := 4;
const ID_THIEF      := 5;
const ID_BARD       := 6;

const EACH_MUST_REPRESENT := 7.5;
const REPRESENT_LEVEL_MOD := 1;
const AVERAGE_SKILL       := 75;
const AVERAGE_LEVEL_MOD   := 15;
const BONUS_PER_LEVEL     := 0.25;

//For book cmd .skill
function GetSkillListName()
    var data := array;
    var bard := array{"Begging", "Peacemaking","Musicianship","TasteIdentification","Provocation","Herding","Cartography","Discordance"};
    var crafter := array{"ArmsLore", "Blacksmithy", "BowcraftFletching", "Lumberjacking", "Mining", "Tinkering", "Tailoring","Carpentry"};
    var mage := array{"Alchemy","ItemIdentification","SpiritSpeak","ResistingSpells","Magery","Meditation","EvaluatingIntelligence","Inscription"};
    var ranger := array{"AnimalLore","Camping","Cooking","Fishing","Archery","Tracking", "Veterinary","AnimalTaming"};
    var thief := array{"DetectingHidden","Hiding","Lockpicking","Snooping","Stealth","Stealing","Poisoning","RemoveTrap"};
    var warrior := array{"Anatomy","Parry", "Healing","Tactics", "Swordsmanship", "MaceFighting", "Fencing", "Wrestling"};

    data[ID_BARD] := bard;
    data[ID_CRAFTER] := crafter;
    data[ID_MAGE] := mage;
    data[ID_RANGER] := ranger;
    data[ID_THIEF] := thief;
    data[ID_WARRIOR] := warrior;

    return data;

endfunction


function GetSkillCapClass(who, skill_id)
    var classid := GetClasseIdForSkill(skill_id);
    var list := GetClasseSkills(classid);
    var skill	:= 0.0;
    var result := 0.0;
    foreach s in ( GetClasseSkills( classid ) )
        skill := GetEffectiveSkill(who,s);
        result := result + skill;
    endforeach

    return result;

endfunction


function GetClassGumpIcon( classid )
    case( classid )
        ID_BARD:		return 5553;
        ID_CRAFTER:	    return 5555;
        ID_MAGE:		return 5569;
        ID_RANGER:	    return 5551;
        ID_THIEF:		return 5575;
        ID_WARRIOR:	    return 5577;
        default:		return 0;
    endcase
endfunction


function GetSkillListId( )
    var data := array;
    var bard := array{SKILLID_BEGGING, SKILLID_PEACEMAKING,SKILLID_MUSICIANSHIP,SKILLID_TASTE_IDENTIFICATION,SKILLID_PROVOCATION,SKILLID_HERDING,SKILLID_CARTOGRAPHY,SKILLID_DISCORDANCE};
    var crafter := array{SKILLID_ARMSLORE, SKILLID_BLACKSMITHY, SKILLID_BOWCRAFT, SKILLID_LUMBERJACKING, SKILLID_MINING, SKILLID_TINKERING, SKILLID_TAILORING,SKILLID_CARPENTRY};
    var mage := array{SKILLID_ALCHEMY,SKILLID_ITEM_IDENTIFICATION,SKILLID_SPIRITSPEAK,SKILLID_RESISTING_SPELLS,SKILLID_MAGERY,SKILLID_MEDITATION,SKILLID_EVALUATING_INTELLIGENCE,SKILLID_INSCRIPTION};
    var ranger := array{SKILLID_ANIMALLORE,SKILLID_CAMPING,SKILLID_COOKING,SKILLID_FISHING,SKILLID_ARCHERY,SKILLID_TRACKING, SKILLID_VETERINARY,SKILLID_TAMING};
    var thief := array{SKILLID_DETECTING_HIDDEN,SKILLID_HIDING,SKILLID_LOCKPICKING,SKILLID_SNOOPING,SKILLID_STEALTH,SKILLID_STEALING,SKILLID_POISONING,SKILLID_REMOVETRAP};
    var warrior := array{SKILLID_ANATOMY,SKILLID_PARRYING, SKILLID_HEALING,SKILLID_TACTICS, SKILLID_SWORDSMANSHIP, SKILLID_MACEFIGHTING, SKILLID_FENCING, SKILLID_WRESTLING};

    data[ID_BARD] := bard;
    data[ID_CRAFTER] := crafter;
    data[ID_MAGE] := mage;
    data[ID_RANGER] := ranger;
    data[ID_THIEF] := thief;
    data[ID_WARRIOR] := warrior;

    return data;

endfunction

function ClasseBonus( who )

	//var level := CInt(GetObjProperty( who, classeid ) );
    var level := CInt(GetObjProperty( who, "classlevel" ) );
	if( level)
		return( 1 + BONUS_PER_LEVEL * level );
	else
		return( 1 );
	endif

endfunction


function ClasseBonusByLevel( level )

	return CDbl( 1.0 + ( BONUS_PER_LEVEL * CDbl(level) ) );

endfunction


function ClasseBonusBySkillId( who, skillid )

	return( ClasseBonusByLevel( IsSpecialisedIn( who, skillid ) ) );

endfunction


function EnumerateRestrictedItemTypesFromClasse( classeid )

	var fract_array := {};
	var restricted_types := {};

	case( classeid )

		CLASSEID_BARD:		fract_array.append( GetPlatemailArmorGraphics() );
					fract_array.append( GetShieldGraphics() );
					break;

		CLASSEID_CRAFTER:	fract_array.append( GetChainmailArmorGraphics() );
					fract_array.append( GetPlatemailArmorGraphics() );
					break;

		CLASSEID_MAGE:		fract_array.append( GetLeatherArmorGraphics() );
					fract_array.append( GetStuddedLeatherArmorGraphics() );
					fract_array.append( GetBoneArmorGraphics() );
					fract_array.append( GetRingmailArmorGraphics() );
					fract_array.append( GetChainmailArmorGraphics() );
					fract_array.append( GetPlatemailArmorGraphics() );
					fract_array.append( GetShieldGraphics() );
					break;

		CLASSEID_RANGER:	fract_array.append( GetPlatemailArmorGraphics() );
					fract_array.append( GetShieldGraphics() );
					break;

		CLASSEID_THIEF:		fract_array.append( GetBoneArmorGraphics() );
					fract_array.append( GetRingmailArmorGraphics() );
					fract_array.append( GetChainmailArmorGraphics() );
					fract_array.append( GetPlatemailArmorGraphics() );
					fract_array.append( GetShieldGraphics() );
					break;

		CLASSEID_WARRIOR:

	endcase


	foreach type in fract_array
		foreach graphic in type
			restricted_types.append( graphic );
		endforeach
	endforeach

	return restricted_types;

endfunction


function GetClasseIdForSkill( skillid )

	foreach id in GetClasseIds()
		if( skillid in GetClasseSkills( id ) )
			return id;
		endif
	endforeach

	return 0;

endfunction


function GetClasseIds()

	return { ID_BARD,ID_CRAFTER,ID_MAGE,ID_RANGER,ID_THIEF,ID_WARRIOR };

endfunction


function GetClasseName( classeid )
    case( classeid )
        ID_BARD:		return( "Бард" );
        ID_CRAFTER:	    return( "Крафтер" );
        ID_MAGE:		return( "Маг" );
        ID_RANGER:	    return( "Лучник" );
        ID_THIEF:		return( "Вор" );
        ID_WARRIOR:	    return( "Воин" );
        default:		return( "Нет класса" );
    endcase
endfunction


function GetClasseSkills( classeid )

	case( classeid )

		ID_BARD:		return { SKILLID_BEGGING,
						 SKILLID_CARTOGRAPHY,
                         SKILLID_DISCORDANCE,
						 SKILLID_HERDING,
						 SKILLID_MUSICIANSHIP,
						 SKILLID_PEACEMAKING,
						 SKILLID_PROVOCATION,
						 SKILLID_TASTE_IDENTIFICATION	};

		ID_CRAFTER:	return { SKILLID_ARMSLORE,
						 SKILLID_BLACKSMITHY,
						 SKILLID_BOWCRAFT,
						 SKILLID_CARPENTRY,
						 SKILLID_LUMBERJACKING,
						 SKILLID_MINING,
						 SKILLID_TAILORING,
						 SKILLID_TINKERING	};

		ID_MAGE:		return { SKILLID_ALCHEMY,
						 SKILLID_EVALUATING_INTELLIGENCE,
						 SKILLID_INSCRIPTION,
						 SKILLID_ITEM_IDENTIFICATION,
						 SKILLID_MAGERY,
						 SKILLID_MEDITATION,
                         SKILLID_RESISTING_SPELLS,
						 SKILLID_SPIRITSPEAK	};

		ID_RANGER:	return { SKILLID_ANIMALLORE,
						 SKILLID_TAMING,
						 SKILLID_ARCHERY,
						 SKILLID_CAMPING,
						 SKILLID_COOKING,
						 SKILLID_FISHING,
						 SKILLID_TRACKING,
						 SKILLID_VETERINARY };

		ID_THIEF:		return { SKILLID_DETECTING_HIDDEN,
						 SKILLID_HIDING,
						 SKILLID_LOCKPICKING,
						 SKILLID_POISONING,
						 SKILLID_REMOVETRAP,
						 SKILLID_SNOOPING,
						 SKILLID_STEALING,
						 SKILLID_STEALTH };

		ID_WARRIOR:	return { SKILLID_ANATOMY,
						 SKILLID_FENCING,
						 SKILLID_HEALING,
						 SKILLID_MACEFIGHTING,
                         SKILLID_PARRYING,
						 SKILLID_SWORDSMANSHIP,
						 SKILLID_TACTICS,
						 SKILLID_WRESTLING };

		default:		return {};

	endcase
endfunction


function GetGeneralSkills()
	return { SKILLID_BEGGING, SKILLID_ARMSLORE, SKILLID_RESISTING_SPELLS, SKILLID_ARCHERY, SKILLID_HIDING, SKILLID_TACTICS };
endfunction


function GetMaxCircle( who )

	foreach classeid in GetClasseIds()
		if( GetObjProperty( who, classeid ) )
			return GetMaxCircleForClasse( classeid );
		endif
	endforeach

	return 0;

endfunction


function GetMaxCircleForClasse( classeid )

	case( classeid )
		ID_BARD:		return 10;
		ID_CRAFTER:	return 6;
		ID_MAGE:		return 0;
		ID_RANGER:	return 8;
		ID_THIEF:		return 0;
		ID_WARRIOR:	return 4;
		default:		return 0;
	endcase

endfunction


function GetSkillPointsMultiplier( who, skillid )

	return ClasseBonusByLevel( IsSpecialisedIn( who, skillid ) );

endfunction


function GetStatPointsMultiplier( who, stat )

	var level   := HaveStatAffinity( who, stat );
	var mult    := 1.0;
	if( level )
		mult  := ClasseBonusByLevel( level );
	else
		level := HaveStatDifficulty( who, stat );
		if( level )
			mult := CDbl( 1.0 / ClasseBonusByLevel( level ) );
		endif
	endif

	return mult;

endfunction


function HaveInvalidSkillEnchantmentForClasse( item, classeid )

	var ret	:= 0;
	var skillnum	:= GetObjProperty( item, "skilladv" );

	if( (skillnum) || (skillnum == 0) && !(skillnum in GetGeneralSkills()) )
		if( !(skillnum in GetClasseSkills( classeid )) )
			ret := 1;
		endif
	endif

	return ret;

endfunction


function HaveRestrictedModEnchantmentForClasse( who, item, classeid )

	var ret := 0;

	case( classeid )
		ID_BARD:		break;
		ID_CRAFTER:	break;
		ID_MAGE:		break;
		ID_RANGER:	break;
		ID_THIEF:		break;
		ID_WARRIOR:	if( GetObjProperty( item, "MagicImmunity" ) )
						ret := 1;
					elseif( GetObjProperty( item, "SpellReflection" ) )
						ret := 1;
					elseif( GetObjProperty( item, "PermMagicImmunity" ) )
						ret := 1;
					elseif( GetObjProperty( item, "Permmr" ) )
						ret := 1;
					endif
					break;

	endcase

	return ret;

endfunction


function HaveStatAffinity( who, stat )

	var classeids;

	case( stat )
		"str":
            classeids := { ID_CRAFTER,ID_WARRIOR };
            break;
		"dex":
            classeids := {  ID_BARD, ID_THIEF };
            break;
		"int":
            classeids := { ID_BARD, ID_MAGE };
            break;
		default:	classeids := {};
	endcase

	foreach id in classeids
		var level := CInt( GetObjProperty( who, id ) );
		if( level )
			return level;
		endif
	endforeach

	return 0;

endfunction


function HaveStatDifficulty( who, stat )

	var classeids, level;

	case( stat )
		"str":
			classeids := {
				ID_BARD,
				ID_MAGE
			}; break;
		"dex":
			classeids := {
			};	break;
		"int":
			classeids := {
				ID_CRAFTER,
				ID_WARRIOR
			};	break;
		default:
			classeids := {
			};
	endcase

	foreach id in classeids
		level := CInt( GetObjProperty( who, id ) );
		if( level )
			return level;
		endif
	endforeach

	return 0;

endfunction


function IsSpecialisedIn( who, skillid )

	var classeid := GetClasseIdForSkill( skillid );
	if( classeid )
		return CInt( GetObjProperty( who, classeid ) );
	endif

endfunction


function IsBard( who )
	return IsFromThatClasse( who, GetClasseSkills( ID_BARD ) );
endfunction
function IsCrafter( who )
	return IsFromThatClasse( who, GetClasseSkills( ID_CRAFTER ) );
endfunction
function IsMage( who )
	return IsFromThatClasse( who, GetClasseSkills( ID_MAGE ) );
endfunction
function IsRanger( who )
	return IsFromThatClasse( who, GetClasseSkills( ID_RANGER ) );
endfunction
function IsThief( who )
	return IsFromThatClasse( who, GetClasseSkills( ID_THIEF ) );
endfunction
function IsWarrior( who )
	return IsFromThatClasse( who, GetClasseSkills( ID_WARRIOR ) );
endfunction


function IsFromThatClasse( who, classe_skills )

	var classe	:= 0;
	var total	:= 0;
	var number := len( classe_skills );

	for i := 0 to (SKILLID__HIGHEST - 10)
		var amount := GetEffectiveSkill( who, i );
		total := total + amount;
		if( i in classe_skills )
			classe := classe + amount;
		endif
	endfor

	if( classe < AVERAGE_SKILL * number )
		return 0;
	elseif( classe < CInt(total * number * EACH_MUST_REPRESENT * 0.01) )
		return 0;
	else
		var level     := 1;
		var represent := EACH_MUST_REPRESENT + REPRESENT_LEVEL_MOD;
		var percent   := CInt( total * number * represent * 0.01 );
		var average_t := CInt( (AVERAGE_SKILL + AVERAGE_LEVEL_MOD) * number );
		while( (classe >= average_t) && (classe >= percent) )
			level     := level + 1;
			represent := CDbl( represent + REPRESENT_LEVEL_MOD );
			percent   := CInt( total * number * represent * 0.01 );
			average_t := CInt( average_t + AVERAGE_LEVEL_MOD * number );
		endwhile

		return level;
	endif

endfunction


function IsProhibitedByClasse( who, item )

	var ret := 0;

	if( GetObjProperty( who, ID_BARD	) )
		if( HaveInvalidSkillEnchantmentForClasse( item, ID_BARD ) )
			ret := 1;
		elseif( item.graphic in EnumerateRestrictedItemTypesFromClasse( ID_BARD ) )
			ret := 1;
		endif

	elseif( GetObjProperty( who, ID_CRAFTER ) )
		if( HaveInvalidSkillEnchantmentForClasse( item, ID_CRAFTER ) )
			ret := 1;
		elseif( GetObjProperty( item, "str" ) )
			ret := 1;
		elseif( item.graphic in EnumerateRestrictedItemTypesFromClasse( ID_CRAFTER ) )
			ret := 1;
		endif

	elseif( GetObjProperty( who, ID_MAGE ) )
		if( HaveInvalidSkillEnchantmentForClasse( item, ID_MAGE ) )
			ret := 1;
		elseif( GetObjProperty( item, "str" ) )
			ret := 1;
		elseif( GetObjProperty( item, "ArBonus" ) )
			ret := 1;
		elseif( item.graphic in EnumerateRestrictedItemTypesFromClasse( ID_MAGE ) )
			ret := 1;
		endif

	elseif( GetObjProperty( who, ID_RANGER ) )
		if( HaveInvalidSkillEnchantmentForClasse( item, ID_RANGER ) )
			ret := 1;
		elseif( item.graphic in EnumerateRestrictedItemTypesFromClasse( ID_RANGER ) )
			ret := 1;
		endif

	elseif( GetObjProperty( who, ID_THIEF ) )
		if( HaveInvalidSkillEnchantmentForClasse( item, ID_THIEF ) )
			ret := 1;
		elseif( item.graphic in EnumerateRestrictedItemTypesFromClasse( ID_THIEF ) )
			ret := 1;
		endif

	elseif( GetObjProperty( who, ID_WARRIOR ) )
		if( HaveInvalidSkillEnchantmentForClasse( item, ID_WARRIOR ) )
			ret := 1;
		elseif( GetObjProperty( item, "int" ) )
			ret := 1;
		elseif( item.graphic in EnumerateRestrictedItemTypesFromClasse( ID_WARRIOR ) )
			ret := 1;
		elseif( HaveRestrictedModEnchantmentForClasse( who, item, ID_WARRIOR ) )
			ret := 1;
		endif
	endif

	return ret;

endfunction


function AssignClasse( character )

	var t;
    //EraseObjProperty( character, CLASSEID_BARD );
    //EraseObjProperty( character, CLASSEID_WARRIOR );
    //EraseObjProperty( character, CLASSEID_CRAFTER );
    //EraseObjProperty( character, CLASSEID_RANGER );
    //EraseObjProperty( character, CLASSEID_MAGE );
    //EraseObjProperty( character, CLASSEID_THIEF );
    EraseObjProperty( character, "classid");
    EraseObjProperty( character, "classlevel");

	foreach classe in GetClasseIds()

		t := IsFromThatClasse( character, GetClasseSkills( classe ) );
		if( t )
            SetObjProperty( character, "classid", classe );
            SetObjProperty( character, "classlevel", t);
            /*if(classe == ID_MAGE)
                SetObjProperty( character, CLASSEID_MAGE, t);
            elseif(classe == ID_WARRIOR)
                SetObjProperty( character, CLASSEID_WARRIOR, t);
            elseif(classe == ID_CRAFTER)
                SetObjProperty( character, CLASSEID_CRAFTER, t);
            elseif(classe == ID_RANGER)
                SetObjProperty( character, CLASSEID_RANGER, t);
            elseif(classe == ID_THIEF)
                SetObjProperty( character, CLASSEID_THIEF, t);
            elseif(classe == ID_BARD)
                SetObjProperty( character, CLASSEID_BARD, t);
            endif*/
		endif
	endforeach

endfunction

function SetCMDClass(who, classid, level)

    var rate := AVERAGE_SKILL;

    who.followers_max := 1;

    var classname := "";
    case (classid)
        ID_RANGER:  who.followers_max := level + 1;
    endcase

    rate:=CInt( (AVERAGE_SKILL + (AVERAGE_LEVEL_MOD * (level - 1))) );

    var cap_array := {};

    //set all skill to "0"
	foreach classe in GetClasseIds()
		foreach sid in GetClasseSkills( classe )
		    if(classid != classe)
                SetAttributeBaseValue(who, GetSkillConfigElem(sid).Attribute, 0);
    	        cap_array[sid+1] := 100;
    	    else
                SetAttributeBaseValue(who, GetSkillConfigElem(sid).Attribute, rate * 10);
                cap_array[sid+1] := AP_GetSkillsCap();
    	    endif
    	endforeach
	endforeach

    SetObjProperty( who, "SkillCaps", cap_array );

    AssignClasse( who );
    RecalcVitals(who);
endfunction
