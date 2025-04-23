let sendables = new Map()
let recievables = new Map()
let order = []

setInterval(update_entries, 700) // ms
setTimeout(register_recievables, 2000) // ms

function update_entries() {
    fetch("http://192.168.4.1/update", {method: "GET", keepalive: true}).then((response) => {
        if (response.status !== 200) {
            let connection_obj = document.getElementById("connection_obj");
            connection_obj.innerHTML = `<div class="offline"><p>Offline</p></div>`
            return;
        } else {
            let connection_obj = document.getElementById("connection_obj");
            connection_obj.innerHTML = `<div class="online"><p>Online</p></div>`
        }
        
        response.text().then((sendable_raw) => {
            let sendable_data = JSON.parse(sendable_raw)
            
            // Iterate through sendables and update values
            Object.entries(sendable_data).forEach((sendable) => {
                if (!sendables.has(sendable[0])) {
                    order.push(sendable[0])

                    let sendables_html = document.getElementById("sendables")
                    
                    if (!sendables_html) {
                        sendables_html.innerHTML = ""
                    }

                    sendables_html.innerHTML += `<div class="data_entry" id="${sendable[0]}""><p class="data_key">${sendable[0]}</p><p class="data_value">${sendable[1].value}</p></div>`
                }

                sendables.set(sendable[0], {"value": sendable[1].value, "type": sendable[1].type})
            })

            update_html()
        }).catch((error) => {
            console.error("Error fetching data:", error);
        })
    }).catch((error) => {
        let connection_obj = document.getElementById("connection_obj");
        connection_obj.innerHTML = `<div class="offline"><p>Offline</p></div>`
        
        console.error("Error fetching data:", error);
    });
}

function update_html() {
    // Iterate through all registered key-values
    order.forEach((key) => {
        const entry = sendables.get(key)

        let element = document.getElementById(key)
        
        element.innerHTML = `<p class="data_key">${key}</p><p class="data_value">${entry.value}</p>`
    })
}

// This is only called once
function register_recievables() {
    fetch("http://192.168.4.1/fetch_recievables", {method:"GET"}).then((response) => {
        response.text().then((recievable_raw) => {
            let recievables_json = JSON.parse(recievable_raw)
            
            Object.entries(recievables_json).forEach((recievable) => {
                let recievables_element = document.getElementById("recievables")

                if (!recievables.has(recievable[0])) {
                    recievables.set(recievable[0], recievable[1])
                }

                if (recievable[1].type.split(">")[1] === "option") {
                    html = `<div class="data_entry" id="${recievable[0]}_option"><p class="data_key">${recievable[0]}</p><select id="${recievable[0]}" class="options_select" onchange="handleInputChange(this)">`;
                    
                    recievable[1].value.options.forEach((option) => {
                        if (option === recievable[1].value.value) {
                            html += `<option class="option" selected>${option}</option>`
                        } else {
                            html += `<option class="option">${option}</option>`
                        }
                    })

                    html += `</select></div>`

                    recievables_element.innerHTML += html
                } else if(recievable[1].type.split(">")[1] === "button") {
                    recievables_element.innerHTML += `<button class="button_entry" id="${recievable[0]}" onclick="onButtonRecievableClick(this)">${recievable[0]}</button>`
                } else {
                    if (recievable[1].type.split(">")[1] === "string") {
                        recievables_element.innerHTML += `<div class="data_entry" id="${recievable[0]}_input"><p class="data_key">${recievable[0]}</p><input class="input" id="${recievable[0]}" onchange="handleInputChange(this)" type="text"></div>`
                    } else {
                        recievables_element.innerHTML += `<div class="data_entry" id="${recievable[0]}_input"><p class="data_key">${recievable[0]}</p><input class="input" id="${recievable[0]}" onchange="handleInputChange(this)" type="number"></div>`
                    }
                }
            })
        }).catch((error) => {
            console.error("Error fetching data:", error)
        })
    }).catch((error) => {
        console.error("Error fetching data:", error)
    })
}

// handles changes to recievable
function handleInputChange(obj) {
    const entry = recievables.get(obj.id)
    let content = obj.id + "\n" + obj.value + "\n" + entry.type + "\n"
    
    if (obj.value === "") {
        return
    }

    fetch("http://192.168.4.1/recievable", {
        method: "PUT",
        body: content,
        headers: {
            "Content-type": 'application/text; charset=UTF-8'
        },
    }).then((response) => {
        if (response.status !== 200) {
            console.error(`There is an error updating recievable, key: ${obj.id}`)
        }
    })
}

// handles button recievables
function onButtonRecievableClick(entry) {
    fetch("http://192.168.4.1/button", {
        method: "PUT",
        body: entry.id,
        headers: {
            "Content-type": 'application/text; charset=UTF-8'
        },
    }).then((response) => {
        if (response.status !== 200) {
            console.error(`There is an error updating recievable, key: ${entry.id}`)
        }
    })
}