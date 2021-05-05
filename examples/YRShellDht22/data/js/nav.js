let nav_navMap = new Map()

function nav_initMap() {
    nav_navMap.set("__idButton_netConfig", { "divId": "__iddiv__netConfig" } )
    nav_navMap.set("__idButton_pinConfig", { "divId": "__iddiv__pinConfig" } )
    nav_navMap.set("__idButton_pinStatus", { "divId": "__iddiv__pinStatus" } )
    nav_navMap.set("__idButton_debug", { "divId": "__iddiv__debug" } )
    nav_navMap.set("__idButton_logs", { "divId": "__iddiv__logs" } )  
    logs_msg("nav_initMap")
}

function nav_divDisable( bid) {
    document.getElementById( nav_navMap.get(bid).divId).style.display = 'none'
    document.getElementById(bid).disabled = false
}

function nav_divEnable( bid) {
    document.getElementById( nav_navMap.get(bid).divId).style.display = ''
    document.getElementById(bid).disabled = true
}

function nav_divSelect( bid) {
    for( let k of nav_navMap.keys()) {
        let o =  nav_navMap.get( k)
        if( k == bid) {
            nav_divEnable( k)
            if( o.hasOwnProperty("focusFunc")) {
                o.focusFunc()
            }
        } else {
            nav_divDisable( k)
            if( o.hasOwnProperty("unfocusFunc")) {
                o.unfocusFunc()
            }
        }
    }
}
  
nav_initMap()
