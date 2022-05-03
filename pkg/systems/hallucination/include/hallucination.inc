use uo;
use os;
use polsys;
use util;

function DeleteObject( mobile, obj )

	var packet := CreatePacket( 0x1D, 5 );
		packet.SetInt32( 1, obj.serial );

	packet.SendPacket( mobile );
endfunction

function DrawObject( mobile, obj, graphic, color, equip_color := 0 )

	var packet := CreatePacket( 0x78, MSGLEN_VARIABLE ),
		new_color := 0,
		new_graphic := 0;

	packet.SetInt32( 3, obj.serial );
	packet.SetInt16( 7, graphic );
	packet.SetInt16( 9, obj.x );
	packet.SetInt16( 11, obj.y );
	packet.SetInt8( 13, obj.z );
	packet.SetInt8( 14, obj.facing );
	packet.SetInt16( 15, color );

	if( obj.IsA( POLCLASS_MOBILE ) && ( obj.graphic in { 400, 401 } ) )
// Might need to move these out of this if statement?
		packet.SetInt8( 17, 0x00 );
		packet.SetInt8( 18, RandomDiceRoll( "1d7" ) );

		foreach item in ListEquippedItems( obj )
			SleepMS( 5 );
			// BYTE[4] Serial
   			// BYTE[2] Graphic
   			// BYTE[1] Layer
   			// BYTE[2] Color (this byte only needed if (Graphic&0x8000)
   			packet.SetInt32( packet.GetSize(), item.serial );
   			if( equip_color )
   				new_color := RandomInt( 99 ) + 2;
   			else
   				new_color := item.color;
   			endif

   			if( !new_color )
   				new_graphic := item.graphic;
   			else
   				new_graphic := item.graphic + 32768;
   			endif

   			packet.SetInt16( packet.GetSize(), new_graphic );
   			packet.SetInt8( packet.GetSize(), item.layer );
   			packet.SetInt16( packet.GetSize(), new_color );
		endforeach
	endif
	packet.SetInt32( packet.GetSize(), 0x00000000 );
	packet.SetInt16( 1, packet.GetSize() );

	packet.SendPacket( mobile );
endfunction