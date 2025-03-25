package com.example;

import akka.actor.AbstractActor;
import akka.actor.ActorRef;
import akka.actor.Props;

public class ActorB extends AbstractActor {

    public static Props props() {
        return Props.create(ActorB.class, ActorB::new);
    }

    @Override
    public Receive createReceive() {
        return receiveBuilder()
                .match(MessageA.class, this::onMessageA)
                .match(String.class, this::onStop)  // Handle stop message
                .build();
    }

    private void onMessageA(MessageA msg) {
        System.out.println("ActorB: Received number " + msg.number + ", sleeping for " + msg.number + " seconds.");
        try {
            Thread.sleep(msg.number * 1000);  // Sleep for the number of seconds received
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        // Send "stop" message back to ActorA after processing the message
        getSender().tell("stop", getSelf());
    }

    private void onStop(String message) {
        if (message.equals("stop")) {
            System.out.println("ActorB: Stopping.");
            getContext().stop(getSelf());  // Stop itself
        }
    }
}


