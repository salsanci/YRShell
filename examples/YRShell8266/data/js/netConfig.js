var netConfig_current
var netConfig_commands = []

function netConfig_init( ) {
    logs_msg("netConfig_init")
    esp_getJson( "jsonNet", netConfig_update)
}

function netConfig_update( o) {
    var name, pwd
    var rc = '<table class="__idClass__netConfigTable">\n'
    rc += '<tr class="__idClass__netConfigRow">'
    rc += '<td class="__idClass__netCellLabel">Host</td>'
    rc += '<td class="__idClass__netCellSsid">' +
    '<input type="text" id="__id__hostSsid" onchange="netConfig_hostSsidChange( this)" maxlength="32" value="' + o.host.hostName + '">' +
    '</td>'
    rc += '<td class="__idClass__netCellPassword">' +
    '<input type="text" id="__id__hostPassword"  onchange="netConfig_hostPasswordChange( this)" maxlength="32" value="' + o.host.hostPassword + '">' +
    '</td>'
    rc += '<td class="__idClass__netCellIp">IP: ' + o.host.hostIp[0] + '.' +  o.host.hostIp[1] + '.' +  o.host.hostIp[2] + '.' +  o.host.hostIp[3] + 
    '</td><td class="__idClass__netCellMac">MAC: ' +
    toHex(o.host.hostMac[0]) + '.' + 
    toHex(o.host.hostMac[1]) + '.' +
    toHex(o.host.hostMac[2]) + '.' +
    toHex(o.host.hostMac[3]) + '.' +
    toHex(o.host.hostMac[4]) + '.' +
    toHex(o.host.hostMac[5]) + '</td>'
rc += '</tr>'
    rc += '<tr><br></tr>'
    rc += '<tr><br></tr>'
    rc += '<tr><br></tr>'
    rc += '<tr><br></tr>'
    rc += '<tr><br></tr>'
    rc += '<tr><br></tr>'

    for( i = 0; i < o.networks.length; i++) {
        if( i < o.networks.length) {
            name = o.networks[ i]
            pwd = '************'
        }
        rc += '<tr class="__idClass__netConfigRow">\n'
        rc += '<td class="__idClass__netCellLabel">' + i + '</td>'
        rc += '<td class="__idClass__netCellSsid">' +
        '<input type="text" id="__id__apSsid' + i + '" onchange="netConfig_ssidChange( this, ' + i + ')" maxlength="32" value="' + o.networks[i].name  + '">' +
        '</td>'
        rc += '<td class="__idClass__netCellPassword">' +
        '<input type="text" id="__id__apPassword' + i + '" onchange="netConfig_passwordChange( this, ' + i + ')" maxlength="32" value="' + o.networks[i].password + '">' +
        '</td>'
        if( i == o.connectedNetwork) {
            rc += '<td class="__idClass__netCellIp">IP: ' + o.networkIp[0] + '.' +  o.networkIp[1] + '.' +  o.networkIp[2] + '.' +  o.networkIp[3] + 
            '</td><td class="__idClass__netCellMac">MAC: ' +
            toHex(o.networkMac[0]) + '.' + 
            toHex(o.networkMac[1]) + '.' +
            toHex(o.networkMac[2]) + '.' +
            toHex(o.networkMac[3]) + '.' +
            toHex(o.networkMac[4]) + '.' +
            toHex(o.networkMac[5]) + '</td>'
        } else {
            rc += '<td <td class="__idClass__netCellIp"></td><td <td class="__idClass__netCellMac"></td>'
        }
        rc += '</tr>'
        netConfig_current = o
    }
    rc += '</table>'
    
    document.getElementById( "__id__netConfigTable").innerHTML = rc
}

function netConfig_hostSsidChange( e) {
    netConfig_current.host.hostName = e.value
    netConfig_current.host.hostNameChange = true
    //logs_msg("netConfig_hostSsidChange: " + JSON.stringify(netConfig_current, null, 2))
}
function netConfig_hostPasswordChange( e) {
    netConfig_current.host.password = e.value
    netConfig_current.host.hostPasswordChange = true
   // logs_msg("netConfig_hostPasswordChange: " + JSON.stringify(netConfig_current, null, 2))
}

function netConfig_ssidChange( e, i) {
    netConfig_current.networks[i].name = e.value
    netConfig_current.networks[i].nameChange = true
    //logs_msg("netConfig_ssidChange: " + i + " " + JSON.stringify(netConfig_current, null, 2))
}
function netConfig_passwordChange( e, i) {
    netConfig_current.networks[i].password = e.value
    netConfig_current.networks[i].passwordChange = true
    //logs_msg("netConfig_passwordChange: " + i + " " + JSON.stringify(netConfig_current, null, 2))

}

function netConfig_submit() {
    var i;
    netConfig_commands = []
    if( netConfig_current.host.hostNameChange) {
        netConfig_commands.push( " s' " + netConfig_current.host.hostName + "' setHostName")
    }
    if( netConfig_current.host.hostPasswordChange) {
        netConfig_commands.push( " s' " + netConfig_current.host.hostPassword + "' setHostPassword")
    }
    for( j = i = 0; i < netConfig_current.networks.length; i++) {
        if( netConfig_current.networks[i].nameChange) {
            netConfig_commands.push( " s' " + netConfig_current.networks[ i].name + "' " + i + " setNetworkName" )
        }
        if( netConfig_current.networks[i].passwordChange ) {
            netConfig_commands.push( " s' " + netConfig_current.networks[i].password + "' " + i + " setNetworkPassword" )
        }
    }
    if( netConfig_commands.length > 0) {
        netConfig_commands.push( "saveNetworkParameters")
        netConfig_command()
    }
}
function netConfig_command( ) {
    if( netConfig_commands.length > 0) {
        var c = netConfig_commands[ 0]
        logs_msg( "netConfig_command: " + c)
        netConfig_commands.shift()
        esp_sendConsole( c, netConfig_command)
    } else {
        logs_msg( "netConfig_command: done")
        setTimeout( netConfig_init, 1)
    }
}

nav_navMap.get("__idButton_netConfig").focusFunc = netConfig_init
logs_msg( "netConfig - init")
