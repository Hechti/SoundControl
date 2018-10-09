package main

import (
	"encoding/json"
	"fmt"
	"log"
	"net/http"
	"strconv"
	"syscall"
)

var (
	lib, _       = syscall.LoadLibrary("SoundControl.dll")
	getVolume, _ = syscall.GetProcAddress(lib, "getVolume")
	isMuted, _   = syscall.GetProcAddress(lib, "isMuted")
	next, _      = syscall.GetProcAddress(lib, "next")
	playpause, _ = syscall.GetProcAddress(lib, "playpause")
	prev, _      = syscall.GetProcAddress(lib, "prev")
	setMute, _   = syscall.GetProcAddress(lib, "setMute")
	setVolume, _ = syscall.GetProcAddress(lib, "setVolume")
	stop, _      = syscall.GetProcAddress(lib, "stop")
)

type Volume struct {
	Volume int `json:"Volume"`
}

type Mute struct {
	Mute bool `json:"Mute"`
}

type Status struct {
	Volume int  `json:"Volume"`
	Mute   bool `json:"Mute"`
}

func homePage(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "Hompage Endpoint Hit")

	status := Status{Volume: 20, Mute: false}

	json.NewEncoder(w).Encode(status)
}

func nextPage(w http.ResponseWriter, r *http.Request) {
	syscall.Syscall(uintptr(next), 0, 0, 0, 0)
}

func prevPage(w http.ResponseWriter, r *http.Request) {
	syscall.Syscall(uintptr(prev), 0, 0, 0, 0)
}

func stopPage(w http.ResponseWriter, r *http.Request) {
	syscall.Syscall(uintptr(stop), 0, 0, 0, 0)
}

func playpausePage(w http.ResponseWriter, r *http.Request) {
	syscall.Syscall(uintptr(playpause), 0, 0, 0, 0)
}

func mutePage(w http.ResponseWriter, r *http.Request) {
	param := r.URL.Query().Get("value")
	if param != "" {
		value, _ := strconv.ParseBool(param)
		var intVal uintptr
		if value {
			intVal = 1
		}
		fmt.Println("mute val = ", intVal)
		syscall.Syscall(uintptr(setMute), 0, intVal, 0, 0)
	} else {
		syscall.Syscall(uintptr(isMuted), 0, 0, 0, 0)
	}

}

func volumePage(w http.ResponseWriter, r *http.Request) {
	param := r.URL.Query().Get("value")
	if param != "" {
		value, _ := strconv.Atoi(param)
		syscall.Syscall(uintptr(setVolume), 0, uintptr(value), 0, 0)
	} else {
		syscall.Syscall(uintptr(getVolume), 0, 0, 0, 0)
	}
}

func handleRequests() {
	http.HandleFunc("/", homePage)
	http.HandleFunc("/next", nextPage)
	http.HandleFunc("/prev", prevPage)
	http.HandleFunc("/stop", stopPage)
	http.HandleFunc("/playpause", playpausePage)
	http.HandleFunc("/mute", mutePage)
	http.HandleFunc("/volume", volumePage)
	log.Fatal(http.ListenAndServe(":8001", nil))
}

func main() {
	defer syscall.FreeLibrary(lib)
	handleRequests()
}
