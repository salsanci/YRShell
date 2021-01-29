
function debug_appendOutput( t) {
    document.getElementById( "__id__debugResult").innerHTML =  document.getElementById( "__id__debugResult").innerHTML + t
}
function debug_appendOutputPre( t) {
    debug_appendOutput( "<pre>" + t + "</pre>")
}
function debug_init( ) {
    document.getElementById( "__id__debugBaseText").value = esp_baseUrl
}

function debug_sendConsole( t) {
    esp_sendConsole( t, debug_appendOutputPre)
}
function debug_getJson( t) {
    esp_getJson( t, function( o) {
        debug_appendOutputPre( JSON.stringify(o, null, 2) )
    })
}
function debug_markOutput( e) {
    debug_appendOutput( "<hr>")
}
function debug_clearOutput( e) {
    document.getElementById( "__id__debugResult").innerHTML = ""
}
function debug_setBase( t) {
    esp_baseUrl = t
}


nav_navMap.get("__idButton_debug").focusFunc = debug_init
logs_msg( "debug load")
