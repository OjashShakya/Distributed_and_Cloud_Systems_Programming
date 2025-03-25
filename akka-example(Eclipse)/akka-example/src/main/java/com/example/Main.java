package com.example;

import akka.actor.ActorRef;
import akka.actor.ActorSystem;

public class Main {
    public static void main(String[] args) {
        ActorSystem system = ActorSystem.create("ActorSystem");

        ActorRef actorARef = system.actorOf(ActorA.props(), "actorA");

        actorARef.tell("start", ActorRef.noSender());
        
        try {
            System.out.println("Press ENTER to terminate the system.");
            System.in.read(); 
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            system.terminate();
        }
    }
}


