
#ifndef _GRID_WORLD_H
#define _GRID_WORLD_H

#include <vector>
#include <iostream>

using std::vector;


class GridWorld {

		// death storage
		// used to find the person who "expired first"
		// a singily linked list that stores the firs person to die 
		// at the front of the list
		// utilizes push back to add ppl who die after
		class DeathStorage{
			// could expand on....
			//
			public:
				int dead_id;
		};
		class Node {
			public:
				DeathStorage data;
				Node* next;

				Node(int _id, Node *_next=nullptr){
					data.dead_id = _id;
					next = _next;
				}
		};

		class District
		{
			public:
				int num_ppl_in_district;
				// ordered by newest member first
				/* vector<int> ppl_ids_in_district; */

				// doubly linked list
				// use push back to order??
				//
				class NodeD{
					public:
						int ppl_ids_in_district;
						NodeD* next;
						NodeD* prev;

						NodeD(int _data, NodeD* _next = nullptr, NodeD *_prev = nullptr){
							ppl_ids_in_district = _data;	
							next = _next;
							prev = _prev;
						}
				};
				NodeD *front_D;
				NodeD *Tail_D;

		};

		// creating a new person
		//
		class Person{
			public:
				bool is_alive;
				int person_id;
				int row_lives;
				int col_lives;
				District::NodeD *backdoorPTR;

				Person(int id, int r, int c, bool _is_alive, District::NodeD *ptr){
					person_id = id;
					is_alive = _is_alive;
					row_lives = r;
					col_lives = c;
					backdoorPTR = ptr;
				}
		};

		int ROWS;
		int COLS;
		/* District *districts[ROWS][COLS]; */

		// want to make 2d vector 
		// need vec bc we dont know size??
		//
		vector<vector<District>> districts;

		// for death storage
		//
	    Node *front; 
		Node *Tail;  

	    // need getters 
	    // 
	    int total_population;
	    int num_ppl_dead; 		

		// needs to be a doubly linked list
		// I can access the index 
		// maybe not....
		// UPDATE: 6/2/21 - 11:45 am
		// I dont need seperate arrays for alive 
		// or dead ppl
		// make it a class attribute
		// and have a signgle vector holding all ppl
		// make ID = to index
		vector<Person> all_ppl;

		// coutner
		//
		int id;

  public:
    /**
    * constructor:  initializes a "world" with nrows and
    *    ncols (nrows*ncols districtcs) in which all 
    *    districtricts are empty (a wasteland!).
    */
    GridWorld(unsigned nrows, unsigned ncols)   {

		// makes the wasteland!!!
		//
		this->districts.resize(nrows, vector<District>(ncols));

		this->total_population = 0;
		this->num_ppl_dead = 0;
		this->id = 0;
		this->ROWS = nrows;
		this->COLS = ncols;
		this->front = nullptr;
		this->Tail = nullptr;

		/* this->front->data.dead_id = 0; */

      // your constructor code here!
    }
    ~GridWorld(){
		// TODO your destructor code here.
		
		// if linked list for death storage is empty
		//
		// remove nodes in linked list (Death storage)
		//
		if(this->front != nullptr)
		{
			Node *cur = this->front;
			while (cur != nullptr)
			{
				cur = cur->next;
				delete this->front;
				this->front = cur;
			}
		}

		// remove the doubly linked list of members 
		// in each district
		//
		for(auto &r : this->districts)
		{
			for(auto &c : r )
			{
				if(c.front_D != nullptr) 
				{
					District::NodeD *cur = c.front_D;
					while (cur)
					{
						cur = cur->next;
						delete c.front_D;
						c.front_D = cur;
					}
				}

			}
		}

    }

	//
	// get_num_ppl_dead
	//
	// get the total num of ppl dead 
	//
	int get_num_ppl_dead() const{
		return this->num_ppl_dead;
	}

	//
	// does_district_exist
	//
	// checks to see if input row and col
	// are valid
	// returns true if vaild input, false otherwise
	//
	bool does_district_exist(int row, int col) const{

		if(row < this->ROWS && col < this->COLS)
			return true;
		return false;
	}

	// 
	// push_back_D
	//
	// pushes the elemnt to the end of the doubly linked list
	// and returns the address of the newly added point
	//
	District::NodeD *push_back_D(District *_thisDist, int data)
	{
		District::NodeD *newN = new District::NodeD(data);
		if(_thisDist->front_D == nullptr)
		{
			newN->prev = nullptr;
			_thisDist->front_D = newN;
			_thisDist->Tail_D = newN;
			return newN;
		}
		_thisDist->Tail_D->next = newN;
		newN->prev = _thisDist->Tail_D;
		_thisDist->Tail_D = newN;
		
		return newN;
	}

    /*
     * function: birth
     * description:  if row/col is valid, a new person is created
     *   with an ID according to rules in handout.  New person is
     *   placed in district (row, col)
     *
     * return:  indicates success/failure
     */
    bool birth(int row, int col, int &id){

		if(does_district_exist(row, col))
		{
			Person *newP = new Person(0, row, col, true, nullptr);
			if(this->front == nullptr) 						// if there is no one dead, there are no ids that need to be reused yet
				/* newP->person_id = this->id++; */
				newP->person_id = this->id++;
			else{ 											// there is someone dead then the next id to be givin out is the front of the list(most old)
				newP->person_id = this->front->data.dead_id;
				Node *cur = this->front;
				this->front = cur->next;
				delete cur;
			} 											
			id = newP->person_id;
			// they get added to their district
			//
			newP->backdoorPTR = push_back_D(&this->districts[row][col], id);

			// they get added to all ppl vec
			//
			this->all_ppl.insert(all_ppl.begin() + id, *newP);

			this->districts[row][col].num_ppl_in_district++;

			this->total_population++;

			delete newP;  // delete here, locally used?

			return true;
		}


      return false; //auto placed, can change
    }

  /**
   * func: push_back
   * status:  DONE
   */
    void push_back(int id) {

		Node *newN = new Node(id);
		newN->data.dead_id = id;
		if(this->front == nullptr){
			this->Tail = newN;
			this->front = newN;
		}
		else{
			this->Tail->next = newN;
			this->Tail = newN;
		}
	
    }

	//
	// deleteD_Node
	//
	// removes the specified node fromt the 
	// doubly linked list of ppl in a district
	//
	void deleteD_Node(District *_thisDist, District::NodeD *delme)
	{
		// nothing to delete
		//
		if(_thisDist->front_D == nullptr || delme == nullptr) return;

		// if requested node is at front of list
		//
		if(_thisDist->front_D == delme)
		{
			_thisDist->front_D = delme->next;
		}

		// for mid deletetion
		//
		if(delme->next != nullptr)
		{
			delme->next->prev = delme->prev;
		}

		// end deletion
		//
		if(delme->prev != nullptr)
		{
			delme->prev->next = delme->next;
			_thisDist->Tail_D = delme->prev;
		}

		delete delme;

	}

    /*
     * function: death 
     * description:  if given person is alive, person is killed and
     *   data structures updated to reflect this change.
     *
     * return:  indicates success/failure
     */
    bool death(int personID){

		if(personID < this->all_ppl.size()){
			Person requested_person = this->all_ppl[personID];
			if(requested_person.is_alive ){

				deleteD_Node(&this->districts[requested_person.row_lives][requested_person.col_lives], requested_person.backdoorPTR);

				this->districts[requested_person.row_lives][requested_person.col_lives].num_ppl_in_district--;

				// they be dead now
				//
				/* requested_person.is_alive = false; */
				/* requested_person.row_lives = -1; */
				/* requested_person.col_lives = -1; */
				this->all_ppl[personID].is_alive = false;
				this->all_ppl[personID].row_lives = -1;
				this->all_ppl[personID].col_lives = -1;
				this->total_population--;
				push_back(personID);	
				return true;
			}
			
		}
      return false; // auto placed, can change 
    }

    /*
     * function: whereis
     * description:  if given person is alive, his/her current residence
     *   is reported via reference parameters row and col.
     *
     * return:  indicates success/failure
     */
    bool whereis(int id, int &row, int &col)const{

		if(id < this->all_ppl.size()){
			Person requested_person = this->all_ppl[id];
			if(requested_person.is_alive){
				row = requested_person.row_lives;
				col = requested_person.col_lives;
				return true;
			}
		}

      return false;
    }

    /*
     * function: move
     * description:  if given person is alive, and specified target-row
     *   and column are valid, person is moved to specified district and
     *   data structures updated accordingly.
     *
     * return:  indicates success/failure
     *
     * comment/note:  the specified person becomes the 'newest' member
     *   of target district (least seniority) --  see requirements of members().
     */
    bool move(int id, int targetRow, int targetCol){
		// if this id is valid 
		// and if this id is alive
		//
		if(id < this->all_ppl.size()){
			Person requested_person = this->all_ppl[id];
			if( requested_person.is_alive )
			{

				// just need to replace seniority
				// could improve by just manipulating the pointers...
				//
				deleteD_Node(&this->districts[requested_person.row_lives][requested_person.col_lives], requested_person.backdoorPTR);
				this->all_ppl[id].backdoorPTR = push_back_D(&this->districts[targetRow][targetCol], id);

				// if they get moved to a different district
				//
				if(requested_person.row_lives != targetRow || requested_person.col_lives != targetCol){
					this->districts[requested_person.row_lives][requested_person.col_lives].num_ppl_in_district--;
					// why doesnt this update the values correctly???
					//
					/* requested_person.row_lives = targetRow; */
					/* requested_person.col_lives = targetCol;  */
					//
					//

					this->all_ppl[id].row_lives = targetRow;
					this->all_ppl[id].col_lives = targetCol;
					this->districts[targetRow][targetCol].num_ppl_in_district++;
				}

				return true;
			}
		}
      return false; // auto placed, can change
    }

	/* creates and populates an integer vector with a snapshot of the */
	/* current residents of district specified by (row, col). The vector is returned as */
	/* a pointer. */
    std::vector<int> * members(int row, int col)const{

		if(does_district_exist(row, col)){

			// malloc new vect?
			vector<int> *current_members = new vector<int>;
			District::NodeD *cur = this->districts[row][col].front_D;
			while(cur != nullptr)
			{
				current_members->push_back(cur->ppl_ids_in_district);
				cur = cur->next;
			}

      		return current_members; 
		}
		
		return nullptr; // auto placed, can change
    }

    /*
     * function: population
     * description:  returns the current (living) population of the world.
     */
    int population()const{
      return this->total_population;
    }
    
    /*
     * function: population(int,int)
     * description:  returns the current (living) population of specified
     *   district.  If district does not exist, zero is returned
     */
    int population(int row, int col)const{
      return this->districts[row][col].num_ppl_in_district;
    }

    /*
     * function: num_rows
     * description:  returns number of rows in world
     */
    int num_rows()const {
      return this->ROWS;
    }

    /*
     * function: num_cols
     * description:  returns number of columns in world
     */
    int num_cols()const {
      return this->COLS;
    }



};

#endif
