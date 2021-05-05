
function pinConfig_genPicture( nr) {
    return '<td class="__idClass__pinCellPicture" rowspan="' + nr + '"><img src="img/ESP8266.png"></td>\n'
}
  
function pinConfig_genSetLeft( p) {
    return pinConfig_genUserNameCell( p) + pinConfig_genTypeCell( p) + pinConfig_genTagCell( p)
}

function pinConfig_genSetRight( p) {
    return pinConfig_genTagCell( p) + pinConfig_genTypeCell( p) + pinConfig_genUserNameCell( p)
}

function pinConfig_genTagCell( p) {
    return '<td class="__idClass__pinCell' + p.Type + '">' + p.Name + '</td>\n'
}

function pinConfig_genTypeCell( p) {
    var rc = ''
    if(p.Modifiable) {
      rc = '<td class="__idClass__pinCell'+ p.Type + '">' +
      '<select id="__idSelect__config' + p.Name + '" value="' + p.Type + '" onchange="pinConfig_typeChange( this, ' + p.Index + ')">' + 
      '<option value="INPUT"' + pinConfig_isSelected( p, "INPUT") + '>INPUT</option>' +
      '<option value="INPUT_PULLUP"' + pinConfig_isSelected( p, "INPUT_PULLUP") + '>INPUT PULLUP</option>' +
      '<option value="OUTPUT"' + pinConfig_isSelected( p, "OUTPUT") + '>OUTPUT</option>' +
      '<option value="OUTPUT_ANALOG"' + pinConfig_isSelected( p, "OUTPUT_ANALOG") + '>OUTPUT ANALOG</option></select>' + 
      '</td>'
    } else {
      rc = '<td class="__idClass__pinCell'+ p.Type + '">' + 
      p.Type +
      '</td>\n'
    }
    return rc
  }
  
function pinConfig_genUserNameCell( p) {  
    return '<td class="__idClass__pinCell' + p.Type + '" id="__id__configPin' + p.Name + 'Name">' + 
      '<input type="text" id="__id__pin' + p.Name + 'Name" onchange="pinConfig_nameChange( this, ' + p.Index + ')" maxlength="32" value="' + p.UserName + '">'+
      '</td>\n'
}

function pinConfig_init( ) {
    global_loadPinConfig()
    var rc = '<table class="__idClass__pinTable">\n'
    rc += '<thead>\n<tr>\n'
    rc += '<td class="__idClass__pinCellHeader"></td>\n'
    rc += '<td class="__idClass__pinCellHeader"></td>\n'
    rc += '<td class="__idClass__pinCellHeader"></td>\n'
    rc += '<td class="__idClass__pinCellHeader"></td>\n'
    rc += '<td class="__idClass__pinCellHeader"></td>\n'
    rc += '<td class="__idClass__pinCellHeader"></td>\n'
    rc += '<td class="__idClass__pinCellHeader"></td>\n'
    rc += '</tr>\n</thead>\n<tbody>\n'    
    np2 = global_pinState.length / 2
    for( i = 0; i < np2; i++) {
        rc += '<tr class="__idClass__pinRow">\n'
        global_pinState[i].Index = i
        global_pinState[ np2 + i].Index = np2 + i
        rc += pinConfig_genSetLeft( global_pinState[i])
        if( i == 0) {
            rc += pinConfig_genPicture( np2)
        }
      rc += pinConfig_genSetRight( global_pinState[ np2 + i]) + '</tr>\n'
    }
    rc += '</table>'
    document.getElementById( "__id__pinConfigTable").innerHTML = rc
}

function pinConfig_isSelected( p, v) {
    var rc = ""
    if( p.Type == v) {
      rc = 'selected="selected"'
    }
    return rc
}

function pinConfig_nameChange( e, i) {
    global_pinState[ i].UserName = e.value
    global_savePinConfig( )
}

function pinConfig_reset( ) {
    global_resetPinConfig()
    global_savePinConfig( )
    pinConfig_init()
}
function pinConfig_setInitial( ) {
    global_setPinsInitialSetting( )
    global_savePinConfig( )
    pinConfig_init()
}

function pinConfig_typeChange( e, i) {
    global_pinState[ i].TypeDirty = true
    global_pinState[ i].Type = e.value
    global_savePinConfig( )
    pinConfig_init()
}

nav_navMap.get("__idButton_pinConfig").focusFunc = pinConfig_init
logs_msg( "pinConfig - init")

