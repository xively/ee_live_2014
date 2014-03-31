package main

import (
	"fmt"
	MQTT "git.eclipse.org/gitroot/paho/org.eclipse.paho.mqtt.golang.git"
	"log"
	"math/rand"
	"net/http"
)

func logRequests(app http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		log.Println(r.Method, r.RequestURI)
		app.ServeHTTP(w, r)
	})
}

func publish(client *MQTT.MqttClient, topic, message string) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		receipt := client.Publish(MQTT.QOS_ZERO, topic, message)
		<-receipt
		w.WriteHeader(200)
	})
}

func main() {
	opts := MQTT.NewClientOptions()
	opts.SetBroker(fmt.Sprintf("tcp://%s:%d", "localhost", 1883))
	opts.SetClientId(fmt.Sprint(rand.Int()))
	opts.SetTraceLevel(MQTT.Off)
	opts.SetCleanSession(true)

	client := MQTT.NewClient(opts)
	_, err := client.Start()
	if err != nil {
		log.Fatal(err)
	}

	http.Handle("/", logRequests(http.FileServer(http.Dir("static"))))
	http.Handle("/vote/red", logRequests(publish(client, "vote", "!red~")))
	http.Handle("/vote/blue", logRequests(publish(client, "vote", "!blue~")))
	http.Handle("/reset", logRequests(publish(client, "reset", "1")))

	log.Println("Starting Server on :8080")
	err = http.ListenAndServe(":8080", nil)
	if err != nil {
		log.Fatal(err)
	}
}
