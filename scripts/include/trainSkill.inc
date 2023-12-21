use uo;
use os;
use attributes;

include "include/skillLists";
include ":attributes:attributes";

var cost;

// This determines the max amount an npc can train
// in any given skill from their skill lists defined in
// the function GetSkillList found in skillLists.inc...
//
// Because we are dealing with the attributes system and everything
// needs to be in in the tenths format, this value should be 100 if
// the train cap is 10.0.
Const MAX_VENDOR_TRAIN := 100; // default: 100 (can train up to 10.0 in a given skill)

function VendorTrain( byref me, byref mobile, byref text )

  var words := SplitWords( text );
  if( words.Size() == 2 )
    ListTrainableSkills( me, mobile );
  else
    var skill := words[3];
    if( words[4] )
      skill += " "+words[4];
    endif
    SelectSkill( me, mobile, Lower( skill ) );
  endif
endfunction

function ListTrainableSkills( byref me, byref mobile )

  var train_list := "";
  foreach skill in ( GetSkillList( me ) )
    if( GetAttributeBaseValue( mobile, AP_SkillIdToAttributeName( FindSkillId( skill ) ) ) >= MAX_VENDOR_TRAIN )
      continue;
    endif

    if( !train_list.Length() )
      train_list += skill;
    else
      train_list += ", "+skill;
    endif
  endforeach

  if( !train_list.Length() )
    Say( "Alas, I cannot teach thee anything." );
    return 0;
  endif

  Say( "I can train the following:" );
  Say( train_list );

  return 1;
endfunction

function SelectSkill( byref me, byref mobile, byref skill )

  var trainable_skills := GetSkillList( me ),
    attribute := AP_SkillIdToAttributeName( FindSkillId( skill ) );
  if( !( skill in trainable_skills ) )
    Say( "'Tis not something I can teach thee of." );
    return 0;
  elseif( GetAttributeBaseValue( mobile, attribute ) >= MAX_VENDOR_TRAIN )
    Say( "I cannot teach thee, for thou knowest all I can teach!" );
    return 0;
  endif

  var diff := MAX_VENDOR_TRAIN - GetAttributeBaseValue( mobile, attribute );
  Say( "I will teach thee all I know, if paid the amount in full.  The price is:"+diff );
  SleepMS( 500 );
  Say( "For less I shall teach thee less." );
  SetObjProperty( mobile, "#VendorTrain", Struct{ "Attribute":=attribute, "Cost":=diff, "Serial":=me.serial, "Time":=ReadGameClock()+120 } );
endfunction

function Train( byref me, byref mobile, byref item )

  var train_info := GetObjProperty( mobile, "#VendorTrain" );
  if( train_info.errortext )
    Say( "I think you need this more than I do friend." );
    MoveItemToContainer( item, mobile.backpack );
    return 0;
  elseif( train_info.Serial != me.serial )
    Say( "You must have me mistaken for someone else." );
    MoveItemToContainer( item, mobile.backpack );
    return 0;
  elseif( train_info.Time < ReadGameClock() )
    Say( "Alas, I have forgotten our last discussion." );
    MoveItemToContainer( item, mobile.backpack );
    EraseObjProperty( mobile, "#VendorTrain" );
    return 0;
  endif

  var paid := 0;
  if( item.amount > train_info.Cost )
    SleepMS( 500 );
    SubtractAmount( item, train_info.Cost );
    Say( "You have overpaid me, here is the difference." );
    MoveItemToContainer( item, mobile.backpack );
    EraseObjProperty( mobile, "#VendorTrain" );
    paid := train_info.Cost;
  elseif( item.amount < train_info.Cost )
    paid := item.amount;
    SetObjProperty( mobile, "#VendorTrain", Struct{ "Attribute":=train_info.Attribute, "Cost":=train_info.Cost-item.amount, "Serial":=me.serial, "Time":=ReadGameClock()+120 } );
    DestroyItem( item );
  else
    paid := train_info.Cost;
    EraseObjProperty( mobile, "#VendorTrain" );
    DestroyItem( item );
  endif

  Say( "Let me show thee something of how this is done." );
  SleepMS( 500 );
  SetAttributeBaseValue( mobile, train_info.Attribute, GetAttributeBaseValue( mobile, train_info.Attribute )+paid );

  return 1;
endfunction

