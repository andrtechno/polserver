use uo;
use os;
use cfgfile;

function HALLUCINATION_GetSettingsCfgFile()

	var cfg := ReadConfigFile( ":hallucination:settings" );

	if( cfg.errortext )
		SysLog( "Error::HALLUCINATION_GetSettingsCfgFile() - Unable to open [:hallucination:settings.cfg] ->"+cfg.errortext );
	endif

	return cfg;
endfunction

function HALLUCINATION_GetSettingsCfgElem( elem_name, byref cfg_file:=0 )

	if( !cfg_file )
		cfg_file := HALLUCINATION_GetSettingsCfgFile();
	endif

	var elem := FindConfigElem( cfg_file, elem_name );
	if( elem.errortext )
		SysLog( "Error::HALLUCINATION_GetSettingsCfgElem() - Unable to find elem ["+elem_name+"] ->"+elem.errortext );
	endif

	return elem;
endfunction
