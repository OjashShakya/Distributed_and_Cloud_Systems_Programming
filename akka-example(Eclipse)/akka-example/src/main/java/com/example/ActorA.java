package com.example;

import akka.actor.AbstractActor;
import akka.actor.ActorRef;
import akka.actor.Props;
import akka.actor.ReceiveTimeout;
import akka.actor.Terminated;
import scala.concurrent.duration.Duration;

import java.util.Random;

public class ActorA extends AbstractActor {

    private ActorRef actorBRef;
    private final Random random = new Random();

    public static Props props() {
        return Props.create(ActorA.class, ActorA::new);
    }

    @Override
    public Receive createReceive() {
        return receiveBuilder()
                .match(ReceiveTimeout.class, this::onReceiveTimeout)
                .match(Terminated.class, this::onTerminated)
                .match(String.class, this::onStart)
                .build();
    }

    @Override
    public void preStart() {
        actorBRef = getContext().actorOf(ActorB.props(), "actorB");
        getContext().watch(actorBRef);
        getContext().setReceiveTimeout(Duration.create(2, "seconds"));
    }

    private void onStart(String message) {
        for (int i = 0; i < 100; i++) {
            int number = random.nextInt(5) + 1;
            System.out.println("ActorA: Sending number " + number + " to ActorB");
            actorBRef.tell(new MessageA(number), getSelf());

            try {
                Thread.sleep(500);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
    }

    private void onReceiveTimeout(ReceiveTimeout timeout) {
        System.out.println("Timeout occurred. ActorB will stop.");
        actorBRef.tell("stop", getSelf()); 
    }

    private void onTerminated(Terminated terminated) {
        System.out.println("ActorB has been terminated, creating new ActorB instance.");
        actorBRef = getContext().actorOf(ActorB.props(), "actorB");
        getContext().watch(actorBRef);
    }
}


