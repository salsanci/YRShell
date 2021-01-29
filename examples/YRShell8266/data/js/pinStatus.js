var pinStatus_pinUpdateRate = 1
var pinStatus_actualPinUpdateRate = 1
var pinStatus_pinReportCount = 0
var pinStatus_pinReportInterval = 5000

var pinStatus_pinUpdateTimer
var pinStatus_pinReportTimer
var pinStatus_lastPinUpdateTime

var pinStatus_minSamplesPerSec = 0.2
var pinStatus_maxSamplesPerSec = 5

var pinStatus_updateRateCoeeficient = 0.2

function pinStatus_genPicture( nr) {
    return '<td class="__idClass__pinCellPicture" rowspan="' + nr + '"><img src="img/ESP8266.png"></td>\n'
}
function pinStatus_genSetLeft( p) {
    return pinStatus_genUserNameCell( p) + pinStatus_genValueCell( p) + pinStatus_genTagCell( p)
}
function pinStatus_genSetRight( p) {
    return pinStatus_genTagCell( p) + pinStatus_genValueCell( p) + pinStatus_genUserNameCell( p)
}
function pinStatus_genTagCell( p) {
    return '<td class="__idClass__pinCell' + p.Type + '">' + p.Name + '</td>\n'
}
function pinStatus_genValueCell( p) {
    let rc = ''
    var val = 0
    if( p.Type == "INPUT" ||  p.Type == "INPUT_PULLUP") {
        val = 0
        if( p && p.hasOwnProperty( "Value")) {
            val = p.Value
        }
        rc = '<td class="__idClass__pinCell' + p.Type + '"><progress class="__idClass__pinProgress" id="__id__pinProgress' + p.Name + '" max="1" value="' + val + '"></td>\n' 
    } else if( p.Type == "INPUT_ANALOG") {
        val = 0
        if( p && p.hasOwnProperty( "Value")) {
            val = p.Value
        }
        rc = '<td class="__idClass__pinCell' + p.Type + '"><progress class="__idClass__pinProgress" id="__id__pinProgress' + p.Name + '"max="1023" value="' + val + '"></td>\n' 
    } else if ( p.Type == "OUTPUT") {
        val = 0
        if( p && p.hasOwnProperty( "Value")) {
            val = p.Value
        }
        rc = '<td class="__idClass__pinCell' + p.Type + '"><input class="__idClass__pinRange" type="range" min="0" max="1" value="' + val +'" onchange="pinStatus_valueChange( this, ' + p.Index + ')"></td>\n' 
    } else if ( p.Type == "OUTPUT_ANALOG") {
        rc = '<td class="__idClass__pinCell' + p.Type + '"><input class="__idClass__pinRange" type="range" min="0" max="1023" value="' + val +'" onchange="pinStatus_valueChange( this, ' + p.Index + ')"></td>\n' 
    } else {
        rc = '<td class="__idClass__pinCell' + p.Type + '">' + 
        p.Type +
        '</td>\n'
    }
    return rc
}
function pinStatus_genUserNameCell( p) {  
    return '<td class="__idClass__pinCell' + p.Type + '">' + p.UserName + '</td>\n'
}
function pinStatus_init( ) {
    logs_msg( "pinStatus_init")
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
        rc += pinStatus_genSetLeft( global_pinState[i])
        if( i == 0) {
            rc += pinStatus_genPicture( np2)
        }
        rc += pinStatus_genSetRight( global_pinState[ np2 + i]) + '</tr>\n'
    }
    rc += '</tbody>\n</table>\n'
    document.getElementById( "__id__pinStatusTable").innerHTML = rc
    pinStatus_pinReportCount = 0
    document.getElementById( "__id__pinStatusProgressUpdate").value = pinStatus_actualPinUpdateRate
    document.getElementById("__id__pinStatusRateLabel").innerHTML = pinStatus_pinUpdateRate + " samples/sec"

    document.getElementById( "__id__pinsStatusUpdateRate").max = pinStatus_maxSamplesPerSec
    document.getElementById( "__id__pinStatusProgressUpdate").max = pinStatus_maxSamplesPerSec

    pinStatus_updatePinUpdateRate( pinStatus_pinUpdateRate)
    esp_processPins( pinStatus_processPinsFirstDone)
}
function pinStatus_processPins() {
    esp_processPins( pinStatus_processPinsDone)
}
function pinStatus_processPinsDone() {
    var d = new Date()
    var t = d.getTime()
    pinStatus_actualPinUpdateRate = (pinStatus_actualPinUpdateRate * (1 - pinStatus_updateRateCoeeficient)) + (pinStatus_updateRateCoeeficient * 1000/( t - pinStatus_lastPinUpdateTime))
    pinStatus_lastPinUpdateTime = t
    if( !isFinite(pinStatus_actualPinUpdateRate) || pinStatus_actualPinUpdateRate < document.getElementById( "__id__pinStatusProgressUpdate") || pinStatus_actualPinUpdateRate > document.getElementById( "__id__pinStatusProgressUpdate").max ) {
        pinStatus_actualPinUpdateRate = pinStatus_pinUpdateRate
    }
    document.getElementById( "__id__pinStatusProgressUpdate").value = pinStatus_actualPinUpdateRate
    pinStatus_pinReportCount += 1
}
function pinStatus_reportPins() {
    var r = 1000*pinStatus_pinReportCount/pinStatus_pinReportInterval
    logs_msg( "Pin status update rate: " + r.toFixed(2) + " samples/sec")
    pinStatus_pinReportCount = 0
}
function pinStatus_processPinsFirstDone() {
    var d = new Date()
    pinStatus_lastPinUpdateTime = d.getTime()
    pinStatus_pinReportCount += 1
}
function pinStatus_leave() {
    logs_msg( "pinStatus_leave")
    clearInterval( pinStatus_pinUpdateTimer)
    clearInterval( pinStatus_pinReportTimer)
}
function pinStatus_valueChange( e, i) {
    global_pinState[ i].Value = e.value
    global_pinState[ i].ValueDirty = true
}
function pinStatus_updatePinUpdateRate( v) {
    if( v < pinStatus_minSamplesPerSec) {
        v = pinStatus_minSamplesPerSec
    }
    logs_msg("pinStatus_updatePinUpdateRate: " + v)
    pinStatus_pinUpdateRate = v
    clearInterval( pinStatus_pinUpdateTimer)
    pinStatus_pinUpdateTimer = setInterval( pinStatus_processPins,  1000/pinStatus_pinUpdateRate)
    pinStatus_actualPinUpdateRate = pinStatus_pinUpdateRate
    document.getElementById( "__id__pinsStatusUpdateRate").value = pinStatus_pinUpdateRate
    document.getElementById("__id__pinStatusRateLabel").innerHTML = pinStatus_pinUpdateRate + " samples/sec"
    document.getElementById( "__id__pinStatusProgressUpdate").value = pinStatus_actualPinUpdateRate
}

nav_navMap.get("__idButton_pinStatus").focusFunc = pinStatus_init
nav_navMap.get("__idButton_pinStatus").unfocusFunc = pinStatus_leave
pinStatus_pinReportTimer = setInterval( pinStatus_reportPins,  pinStatus_pinReportInterval)
logs_msg( "pinStatus - load")
