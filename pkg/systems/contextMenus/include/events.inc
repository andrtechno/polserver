use uo;
use os;
use unicode;
use cliloc;

include "include/sysEvent";
include "include/client";
include ":gumps:confirmationSizable";
include ":insurance:insurance";
/*
 *
 * ContextMenu_SendNPCEvent( mobile, npc, speech_event )
 *
 * Parameters:
 * mobile - player ref accessing context menu
 * npc - npc to send event to
 * str - string to send to within the event struct.
 *
 * Purpose:
 * Sends an event to the npc ai script.
 *
 * Date: 2022-07-02 18:09:41
 * Shard Name: Pangaea
 *
 * Author: Oden (ThisIsMe)
 * Email: DavidJSGardner@Gmail.com
 * ICQ: 625-658-845
 * Skype: DavidJSGardner
 * Discord: ThisIsMe#7491
 *
 * Return Values:
 * none
 *
 */
function ContextMenu_SendNPCEvent( byref mobile, byref npc, byref str )

	var event := SendEvent( npc, Struct{ "source":=mobile, "text":=npc.name+" "+str, "type":=EVID_SPEECH } );
	if( event.errortext )
		Print( "ERROR::ContextMenu_SendNPCEvent -> "+event.errortext );
	endif
endfunction

function ContextMenu_SendMerchantNodeEvent( byref mobile, byref npc, byref str )

    var node_serial := GetObjProperty( npc, "MerchantNode" );
    if( node_serial.errortext )
        PrintTextAbove( npc, "Context Menus are  broken for me.", 0x3, 0x22 );
        return 0;
    endif
    //print(Struct{ "source":=mobile, "npc":=npc, "text":=npc.name+" "+str, "type":=SYSEVENT_SPEECH });
    var node := SystemFindObjectBySerial( node_serial );
    var process := GetProcess( GetObjProperty( node, "#Pid" ) );
    var event := process.SendEvent( Struct{ "source":=mobile, "npc":=npc, "text":=str, "type":=SYSEVENT_SPEECH } );
    if( event.errortext )
        Print( "ERROR::ContextMenu_SendMerchantNodeEvent -> "+event.errortext );
    endif
endfunction

/*
 *
 * ContextMenu_OpenPack( byref mobile, byref obj )
 *
 * Parameters:
 * mobile - player
 * obj - mobile_ref of the owner of the backpack being accessed.
 * Parameter_2_Name
 *
 * Purpose:
 * Opens backpack
 *
 * Date: 2022-07-02 18:37:40
 * Shard Name: Pangaea
 *
 * Author: Oden (ThisIsMe)
 * Email: DavidJSGardner@Gmail.com
 * ICQ: 625-658-845
 * Skype: DavidJSGardner
 * Discord: ThisIsMe#7491
 *
 * Return Values:
 * Return_Value_Type - Description_of_return_value
 *
 */
function ContextMenu_OpenPack( byref mobile, byref obj )

	if( mobile == obj )
		UseItem( mobile.backpack, mobile );
	elseif( mobile != obj )
		if( obj.master == mobile )
			SendOpenSpecialContainer( mobile, obj.backpack );
		elseif( mobile.cmdlevel > obj.cmdlevel )
			SendOpenSpecialContainer( mobile, obj.backpack );
		endif
	endif

	return 1;
endfunction


function ContextMenu_RemovePincode( byref mobile, byref obj )

    var rem := SmallYesNo( mobile, "Remove Pincode?" );
    if(!rem)
        SendSysMessage(mobile, "Cancelled.", color := 33);
        return 0;
    endif

    SendSysMessageUC(mobile, "Пин-код успешно удален.", "RUS", 3, MESSAGE_COLOR_GOOD);
    EraseObjProperty(obj, "pincode");
    return 1;
endfunction

function ContextMenu_SetPincode( byref mobile, byref obj )

    var entry_pin := SendTextEntryGump( mobile, "Entry pincode", cancel := TE_CANCEL_ENABLE, style := TE_STYLE_NORMAL, maximum := 4, line2 := "" );
    if(!entry_pin)
        SendSysMessageUC(mobile, "Отмена.", "RUS", 3, MESSAGE_COLOR_BAD);
        return 0;
    endif
    //if(!validateSetPin(mobile, entry_pin))
    //return 0;
    //endif

    SendSysMessageUC(mobile, "Пин-код успешно установлен.", "RUS", 3, MESSAGE_COLOR_GOOD);
    SetObjProperty(obj, "pincode", entry_pin);
    SetObjProperty(obj, "owner", mobile.serial);


	return 1;
endfunction

function ContextMenu_InsureItem( byref mobile )

	while( 1 )
		SendSysMessageCL(mobile, 1060868, color := 88 ); // Target the item you wish to toggle insurance status on <ESC> to cancel
		var ins_item := Target( mobile );
		if( !ins_item )
			SendSysMessageCL( mobile, 1042023, color := 33 ); // Cancelled.
			return 0;
		endif
		if( !((ins_item.container == mobile.backpack) || (ins_item in ListEquippedItems( mobile ))) )
			SendSysMessageCL( mobile, 1060871, font := 2, color := 33 ); // You can only insure items that you have equipped or that are in your backpack
			continue;
		endif
		if( (ins_item.stackable) || (ins_item.cursed) || (ins_item.IsA( POLCLASS_CONTAINER )) && !ins_item.IsQuiver() )
			SendSysMessageCL( mobile, 1060869, color := 33 ); // You cannot insure that
			continue;
		elseif( ins_item.newbie )
			SendSysMessageCL( mobile, 1060870, color := 56 ); // That item is blessed and does not need to be insured
			continue;
		endif
		if ( !ins_item.insured )
			InsureItem( mobile, ins_item );
			SendSysMessageCL(mobile, 1060873, color := 66 ); // You have insured the item
		else
			UninsureItem( mobile, ins_item );
			SendSysMessageCL( mobile, 1060874, color := 40 ); // You cancel the insurance on the item
		endif
	endwhile

endfunction

function ContextMenu_InsuranceMenu( byref mobile )

	if( !Run_Script( ":insurance:commands/player/insurancemenu", {mobile} ) )
		Print("Could not run script");
	endif

endfunction

function ContextMenu_HandleJoinEvent( byref mobile, byref obj )

    if( obj.IsClassMaster() )
        ContextMenu_SendNPCEvent( mobile, obj, CM_JOIN );
    endif
endfunction

function ContextMenu_HandleResignEvent( byref mobile, byref obj )

    if( obj.IsClassMaster() )
        ContextMenu_SendNPCEvent( mobile, obj, CM_RESIGN );
    endif
endfunction