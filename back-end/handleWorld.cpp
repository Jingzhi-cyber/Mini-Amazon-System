#include "handleWorld.hpp"

void initProductsAmount(){
  Server& server = Server::getInstance();
  // buy some initial products
  for (auto const& warehouse : server.WH_list) {
    ACommands acommand;
    APurchaseMore* apurchase = acommand.add_buy();
      Server& server = Server::getInstance();
      int seq_num = server.getSeqNum();
      apurchase->set_seqnum(seq_num);
      apurchase->set_whnum(warehouse.wh_id);
    for (auto const& product : warehouse.products) {
      AProduct* aproduct = apurchase->add_things();
      aproduct->set_id(product.p_id);
      aproduct->set_count(product.p_num);
      aproduct->set_description(product.p_name);
    }
    trySendMsgToWorld(acommand, seq_num);
  }


  // for (auto const& warehouse : server.WH_list) {
  //     for (auto const& product : warehouse.products) {
  //         std::cout << "start to purchaseMore: " <<   std::endl;
  //         std::thread t_purchase(purchaseMore, warehouse.wh_id, product.p_id, product.p_name,
  //                       product.p_num);
  //                       t_purchase.detach();
  //     }
  // }



  // auto const& warehouse = server.WH_list[0];
  // auto const& product = warehouse.products[0];
  // std::cout << "start to purchaseMore: " <<   std::endl;
  // purchaseMore(warehouse.wh_id, product.p_id, product.p_name, product.p_num);
  // std::thread t_purchase(purchaseMore, warehouse.wh_id, product.p_id, product.p_name,
  //           product.p_num);
            // t_purchase.detach();
  // TODO: wait for all threads finishing
}

void handleWorldResponse(AResponses& aresponses){
      Server& server = Server::getInstance();
      for (int i = 0; i < aresponses.acks_size(); i ++ ){
        std::cout << "aresponses.acks(i) is " << aresponses.acks(i) << std::endl;
        server.finished_SeqNum_set.insert(aresponses.acks(i));
      }
    
      std::cout << "start to parse APurchaseMore" << std::endl;
      // start to parse APurchaseMore
      ACommands APurchaseMore_ack;
      for (int i = 0; i < aresponses.arrived_size(); i ++ ){
        APurchaseMore arrived = aresponses.arrived(i);
        int seqnum = arrived.seqnum();
        std::cout << "arrived.seqnum() is " << seqnum << std::endl;
        if (server.finished_SeqNum_set.find(seqnum) != server.finished_SeqNum_set.end()){
          continue;
        }
        processPurchaseMore(arrived);
        APurchaseMore_ack.add_acks(seqnum);
      }
      server.A2W_send_queue.push(APurchaseMore_ack);

      ACommands APacked_ack;
      std::cout << "start to parse APacked" << std::endl;
      // start to parse APacked
      for (int i = 0; i < aresponses.ready_size(); i ++ ){
        APacked ready = aresponses.ready(i);
        int seqnum = ready.seqnum();
        if (server.finished_SeqNum_set.find(seqnum) != server.finished_SeqNum_set.end()){
          continue;
        }
        processPacked(ready);
        APacked_ack.add_acks(seqnum);
      }
      server.A2W_send_queue.push(APacked_ack);

      std::cout << "start to parse ALoaded" << std::endl;
      ACommands ALoaded_ack;
      // start to parse ALoaded
      for (int i = 0; i < aresponses.loaded_size(); i ++ ){
        ALoaded loaded = aresponses.loaded(i);
        int seqnum = loaded.seqnum();
        if (server.finished_SeqNum_set.find(seqnum) != server.finished_SeqNum_set.end()){
          continue;
        }
        processLoaded(loaded);
        ALoaded_ack.add_acks(seqnum);
      }
      server.A2W_send_queue.push(ALoaded_ack);
}

void purchaseMore(const int wh_id, const int p_id, const std::string p_name, const int p_num){
  std::cout << "start to purchaseMore: at wh_id " << wh_id << " of p_id " << p_id <<  std::endl;
  ACommands acommand;
  APurchaseMore* apurchase = acommand.add_buy();
  AProduct* aproduct = apurchase->add_things();
  aproduct->set_id(p_id);
  aproduct->set_count(p_num);
  aproduct->set_description(p_name);
  apurchase->set_whnum(wh_id);
  Server& server = Server::getInstance();
  int seq_num = server.getSeqNum();
  apurchase->set_seqnum(seq_num);
  trySendMsgToWorld(acommand, seq_num);
}

void processPurchaseMore(APurchaseMore& apurchasemore){
    // parse whnum, products
    Database& db = Database::getInstance();
    std::cout << "start to processPurchaseMore" << std::endl;
    int wh_id = apurchasemore.whnum();
    for (int i = 0; i < apurchasemore.things_size(); i ++ ){
      long p_id = apurchasemore.things(i).id();
      std::string p_name = apurchasemore.things(i).description();
      int p_num = apurchasemore.things(i).count();
      // add this product to Inventory
      std::cout << "add " << p_id << " this product to Inventory " << wh_id << std::endl;
      db.insert_and_update_inventory(wh_id, p_id, p_num);
    }
    std::cout << "success add products into warehouse" << std::endl;
}

void processPacked(APacked& apacked){
  std::cout << "start to processPacked" << std::endl;
  int ship_id = apacked.shipid();
  Database& db = Database::getInstance();
  std::string status = "packed";
  db.update_package_status(ship_id, status);
}

void processLoaded(ALoaded& aloaded){
  std::cout << "start to processLoaded" << std::endl;
  int ship_id = aloaded.shipid();
  Database& db = Database::getInstance();
  std::string status = "loadeded";
  db.update_package_status(ship_id, status);
  // try send AUreqDelivery to UPS
}

void trySendMsgToWorld(ACommands& ac, int seq_num){
  // periodically thread, for at least once
  std::cout << "try to send MSG, for at least once: " <<  std::endl;
  Server& server = Server::getInstance();
  while (1){
      // std::cout << "start to periodically thread" <<  std::endl;
      server.A2W_send_queue.push(ac);
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      if (server.finished_SeqNum_set.find(seq_num) != server.finished_SeqNum_set.end()){
        break;
      }
  }
  std::cout << "finished to periodically thread" <<  std::endl;
}
