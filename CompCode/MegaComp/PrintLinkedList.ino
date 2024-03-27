void PrintLinkedList(void) {
  struct node *tmp = directions.head;
  if(tmp == nullptr){
    Serial.println("NULL");
    Serial.println();
  }

  while(tmp!=nullptr){
    Serial.print(tmp->action[0]);
    Serial.print('\t');
    Serial.println(tmp->action[1]);
    tmp = tmp->next;
  }
  Serial.println();
}