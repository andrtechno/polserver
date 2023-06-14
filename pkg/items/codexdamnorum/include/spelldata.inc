
include ":damage:damage";
include ":charclasses:classes";

// Immunities constants
const IMMUNED	:= 1;
const CURSED	:= 2;

function ModifyWithMagicEfficiency( who, val )

	if( GetObjProperty( who, CLASSEID_MAGE ) )
		val := CInt(val * ClasseBonus( who ));
	elseif( GetObjProperty( who, CLASSEID_WARRIOR ) )
		val := CInt(val / ClasseBonus( who ));
	endif

	var penalty := CInt( GetMagicEfficiencyPenalty( who ) );
	if( penalty >= 100 )
		return 0;
	endif

	return CInt( val * ( 100 - penalty ) / 100 );

endfunction

function IsProtected( caster, cast_on, circle )

	var protserial := GetObjProperty( cast_on, "MagicImmunity" );
	var protlvl	 := Cint(GetObjProperty( cast_on, "PermMagicImmunity" ));
	if( !protlvl )
		protlvl := 0;
	endif

	circle := circle * ClasseBonus( cast_on );

	var cmi := 0 - protlvl;
	var item;
	var charges;
	var cursed;

	if( protserial )
		foreach thing in ListEquippedItems( cast_on )
			if( thing.serial == protserial )
				item := thing;
				charges := CInt( GetObjProperty( item, "MagicImmunity" ) );
				cursed := GetObjProperty( item, "Cursed" );
				if( !charges )
					EraseObjProperty( cast_on, "MagicImmunity" );
					EraseObjProperty( item, "MagicImmunity" );
					PrintTextAbovePrivate(item, CStr(item.desc + " is out of charges!"), cast_on );
					CheckForAnotherProtItem( cast_on, item, "MagicImmunity" );
				endif
				break;
			endif
		endforeach
	endif

	if( charges >= circle )
		charges := charges - circle;
		SetObjProperty( item, "MagicImmunity", charges );
		if( !cursed )
			PrintTextAbovePrivate( cast_on, CStr("Your " + item.desc + " protected you from the spell!"), cast_on );
			return IMMUNED;
		else
			return CURSED;
		endif

	elseif( cmi >= circle )
		return CURSED;

	elseif( protlvl >= circle )
		PrintTextAbovePrivate( cast_on, CStr(caster.name + "'s spell is absorbed by your magical protection!"), cast_on );
		PrintTextAbovePrivate( cast_on, CStr("The spell dissipates upon contact with " + cast_on.name + "'s magical barrier!"), caster );
		return IMMUNED;
	endif

	return 0;

endfunction

function BurnSelf( caster, circle )

	if(caster.dead) return; endif

	var dmg;
	var cast_on := caster;
    dmg := CalcSpellDamage( caster, cast_on, circle );
    if( GetObjProperty( caster, CLASSEID_MAGE ) )
        dmg := CInt( dmg / ClasseBonus( caster ) );
    elseif( GetObjProperty( caster, CLASSEID_WARRIOR ) )
        dmg := CInt( dmg * ClasseBonus( caster ) );
    endif

	ApplyRawDamage( caster, Resisted( caster, circle, cast_on, CInt(dmg / 2) ) );

endfunction


function ExtractName( name )

	var words := SplitWords( name );

	//Trunc the articles
	if( (words[1] == "A") || (words[1] == "a") || (words[1] == "An") || (words[1] == "an") )
		name := words[2];
		var i := 3;
		for( i; i <= len(words); i := i + 1 )
			name := name + " " + words[i];
		endfor
	endif


	//Remove the "corpse of" from the name
	words := SplitWords( name );
	if( (words[1] == "corpse") && (words[2] == "of") )
		name := words[3];
		var i := 4;
		if( words[3] == "a" )
			name := words[4];
			i := 5;
		endif

		for( i; i <= len(words); i := i + 1)
			name := name + " " + words[i];
		endfor
	endif


	//Remove the "Reanimated" or "conjured" from the name
	words := SplitWords( name );
	if( (words[1] == "Reanimated") || (words[1] == "conjured") )
		name := words[2];
		var i := 3;
		for( i; i <= len(words); i := i + 1 )
			name := name + " " + words[i];
		endfor
	endif

	if( !name )
		name := "creature";
	endif

	return name;

endfunction


function GetMagicEfficiencyPenalty( who )

	var penalty := 0;

	if(who.dead) return 0; endif

	if( who.isA(POLCLASS_NPC) ) return 0; endif

	foreach item in ListEquippedItems( who )
		if( GetObjProperty( item, "MagicPenalty" ) )
			penalty := penalty + GetObjProperty( item, "MagicPenalty" );
		endif
	endforeach

	if( penalty > 100 )
		penalty := 100;
	endif

	return CInt(penalty);

endfunction