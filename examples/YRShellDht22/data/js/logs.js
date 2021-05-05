var logs_content = ""

function logs_getFormattedDate() {
    d = new Date();
    var year = "0000" + d.getFullYear()
    var month = "0" + (d.getMonth() + 1)
    var day = "0" + d.getDay()
    var hour = "0" + d.getHours()
    var minutes = "0" + d.getMinutes()
    var seconds = "0" + d.getSeconds()
    var ms = "00" + d.getMilliseconds()

    var rc = year.substr(-4) + "-" + month.substr(-2) + "-" + day.substr(-2) + "_"  + hour.substr( -2) + ":" + minutes.substr(-2) + ":" + seconds.substr(-2) + "." + ms.substr(-3)
    return rc
}

function logs_msg( m) {
    var dm =  logs_getFormattedDate() + " " + m + "\n"
    logs_content += dm
    console.log( dm)
    var e = document.getElementById( "__id__logsContent")
    if( e) {
        e.innerHTML = logs_content
    }
}
function logs_reset() {
    logs_content = ""
    logs_msg("logs_reset")
}
function logs_init() {
    logs_msg( "logs_init")
}

function logs_leave() {
    logs_msg( "logs_leave")
}

setTimeout( function() {
    nav_navMap.get("__idButton_logs").focusFunc = logs_init
    nav_navMap.get("__idButton_logs").unfocusFunc = logs_leave
    }, 500)
logs_msg( "logs load")
