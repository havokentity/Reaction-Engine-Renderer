/*»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
+
+	Author	: Varun R
+	Started	: 21st January, 2008
+	Revision	: 0
+	Comments	: Utility functions and classes.
+
+			Copyright © differential Arts	2007-09
+			www.differentialarts.com
  ««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««*/


#ifndef	_RXUTIL_H_
#define	_RXUTIL_H_

#define	RX_LIST_BEGIN			-0xef
#define	RX_LIST_END				-0xfe
#define	RX_LIST_DEFAULT_SIZE	16


#ifdef	SCN_EXPORT
	#define	LOG		rxSceneEngine ->mlog
#else
	#define	LOG		myGlobalGame ->mlog
#endif

namespace	DifferentialArts
{

	namespace Containers
	{
		/*!
			The base container class. Does nothing on its own. Derived by
			every specialized container implementation.
		*/
		template	<class genericKey, class genericValue>
		class	Container
		{
		protected:

			struct	Node
			{
				int				prev, next;		//!<Indices of the previous and next node.
				genericKey		key;			//!<The identifier key.
				genericValue	value;			//!<Corresponding value.
			};

		public:

			class		Iterator
			{
			private:
				int			curIndex;		//!<Current node being accessed.
				Container<genericKey, genericValue>
							*list;			//!<The list we're traversing.
			public:
				Iterator(void)	: curIndex(-1)	{}

				void	SetList(Container<genericKey, genericValue> &lst)
				{
					list = & lst;
					curIndex = lst.GetStartIndex();
				}

				Iterator(Container<genericKey, genericValue> &lst)
				{
					SetList(lst);
				}

				//Fast version without bound checks//
				inline  Iterator	&operator ++ (void)	//Pre increment.
				{
					curIndex = list ->nodes[curIndex].next;
					return (*this);
				}

				inline Iterator	&operator ++ (int)
				{
					return ++(*this);
				}



				inline Iterator	&operator += (int offset)
				{
					for(int i=0; i<offset && curIndex >= 0; i++)
						curIndex = list ->nodes[curIndex].next;

					return	(*this);
				}

				inline Iterator	operator + (int offset)
				{				
					Iterator	itr = *this;

					return	(itr += offset);
				}



				inline Iterator	&operator -= (int offset)
				{
					for(int i=0; i<offset && curIndex >= 0; i++)
						curIndex = list ->nodes[curIndex].prev;

					return	(*this);
				}

				inline Iterator	operator - (int offset)
				{				
					Iterator	itr = *this;

					return	(itr -= offset);
				}
				


				inline Iterator &	operator -- (void)	//Pre increment.
				{
					curIndex = list ->nodes[curIndex].prev;
					return (*this);
				}

				inline Iterator	& operator -- (int)
				{
					--(*this);

					return itr;
				}

				inline int	GetCurIndex(void)	const
				{
					return curIndex;
				}

				inline genericKey		GetKey(void)	const
				{
					return	list ->nodes[curIndex].key;
				}

				inline genericValue	 &GetValue(void)	const
				{
					return	list ->nodes[curIndex].value;
				}

				//!<Returns NULL if curIndex is out of bounds else the pointer to the value.
				inline genericValue *	operator -> (void) const
				{
					if(curIndex < 0)
						return NULL;

					return	& ((list ->nodes) + curIndex) ->value;
				}

				inline Node	*GetNode(void)	const
				{
					return	((list ->nodes) + curIndex);
				}

				friend class Container;
			};

			int	GetStartIndex(void)	const
			{
				return	startNode;
			}

			int	GetLastIndex(void)	const
			{
				return	endNode;
			}

			unsigned int GetSize(void)	const
			{
				return	numNodes;
			}

			genericValue	&GetValueAtIndex(unsigned int index)
			{
				return	nodes[index].value;
			}

			Container(void) : nodes(NULL)	{}

		protected:

			unsigned int	numNodes;			//!<Number of elements currently handled.
			int				startNode;			//!<Index of the first element.
			int				endNode;			//!<Index of the last element.
			Node			*nodes;				//!<The actual list of items.

		public:
			bool			autoGrow;			//!<Automatically increase capacity to accomodate elements when full if true.
		};


		/*!
			A special fast sorted list class using static arrays optimized
			for quick insertion and deletion as well. Lookup is tediously
			slow. A generic key is used to sort in ASCENDING order.

		*/
		template	<class genericKey, class genericValue>
		class	SortedList : public	Container<genericKey, genericValue>
		{
		public:

			bool	Init(unsigned int size)
			{
				Kill();	//Just in case.

				freeList = new unsigned int[size];

				if(! freeList)
				{
					LOG.LogError( LString("Failed to initialize freeList of size[%d].", size) );
					return false;
				}

				nodes = new Node[size];

				if(! nodes)
				{
					LOG.LogError( LString("Failed to initialize nodes of size[%d].", size) );

					delete	[]freeList;
					freeList = NULL;

					return false;
				}

				numFree = size;
				numNodes = 0;
				startNode = endNode = RX_LIST_BEGIN;

				//Set initial freeList values.
				for(unsigned int i=0; i<numFree; i++)
					freeList[i] = i;

				return true;
			}

			void	Kill(void)
			{
				if(freeList)
				{
					delete	[]freeList;
					freeList = NULL;
				}

				if(nodes)
				{
					delete	[]nodes;
					nodes = NULL;
				}

				numFree = numNodes = 0;
				startNode = endNode = RX_LIST_BEGIN;
			}

			SortedList(void)	: freeList(NULL)
			{
				Init(RX_LIST_DEFAULT_SIZE);
				autoGrow = true;
			}

			SortedList(unsigned int size)
			{
				Init(size);
				autoGrow = true;

			}

			~SortedList()
			{	Kill();	}


			bool	AddNode(genericKey key, genericValue value)
			{
				if(! numFree)
				{
					if(! autoGrow)	//Cannot grow the storage.
						return false;

					//Need to increase allocation.
					unsigned int	*newFL;
					Node			*newNodes;
					unsigned int	size = numNodes;
					unsigned int	newSize = size * 2;		//We exponentially increase by factor of 2 each time.

					newFL = new unsigned int[newSize];

					if(! newFL)
					{
						LOG.LogError( LString("Failed to initialize new freeList of size[%d].", newSize) );
						return false;
					}

					newNodes = new Node[newSize];

					if(! newNodes)
					{
						LOG.LogError( LString("Failed to initialize new node list of size[%d].", newSize) );

						delete	[]newFL;
						newFL = NULL;

						return false;
					}
			

					//Copy the nodes.
					for(unsigned int i=0; i<size; i++)
						newNodes[i] = nodes[i];

					//And now the free list.
					for(unsigned int i=0; i<numFree; i++)
						newFL[i] = freeList[i];

					//Also add in the extra freelist entries.
					for(unsigned int i=numFree, j=size; j<newSize; i++, j++)
						newFL[i] = j;

					numFree += size;
					size = newSize;
					
					delete	[]freeList;
					delete	[]nodes;

					freeList = newFL;
					nodes = newNodes;
				}


					
				numFree--;
				numNodes++;
				Node	&node = nodes[freeList[numFree]];

				node.key = key;
				node.value = value;

				if(numNodes == 1)		//First element.
				{
					node.prev = RX_LIST_BEGIN;
					node.next = RX_LIST_END;
					startNode = freeList[numFree];
					endNode = freeList[numFree];

					return true;
				}


				//More than 1 element, so sort and place.
				Iterator	itr(*this);
				unsigned int	i;

				for(i=0; i<numNodes-1 && itr.GetNode() ->key < key; i++, ++itr);

				if(i < numNodes-1)	//Somewhere in between.
				{
					//Set prev.
					if(itr.GetNode() ->prev != RX_LIST_BEGIN)	//Not the first element.
					{
						node.prev = itr.GetNode() ->prev;
						node.next = nodes[itr.GetNode() ->prev].next;

						nodes[itr.GetNode() ->prev].next = freeList[numFree];
						itr.GetNode() ->prev = freeList[numFree];
					}
					else		//The first element.
					{
						node.prev = RX_LIST_BEGIN;
						node.next = startNode;

						itr.GetNode() ->prev = freeList[numFree];
						startNode = freeList[numFree];
					}
				}
				else		//The last element.
				{
					node.prev = endNode;
					nodes[endNode].next = freeList[numFree];

					node.next = RX_LIST_END;
					endNode = freeList[numFree];
				}

				return true;
			}

			
			/*!
				Traverses the list to find the element at index 'index' and deletes it.
			*/
			bool	DeleteWithLookup(int index)
			{
				if(index < 0 || index >= numNodes)
					return false;

				unsigned int ind = startNode;

				for(int i=0; i<index; i++)
				{
					ind = nodes[ind].next;
					if(ind == RX_LIST_END)
						return false;	//End reached and not found.
				}

				return	Delete((int) ind);
			}


			/*!
				Deletes the element at given raw index.
			*/
			bool	Delete(int index)
			{
				if(index < 0)
					return false;

				if(nodes[index].prev != RX_LIST_BEGIN)	//Not first.
					nodes[nodes[index].prev].next = nodes[index].next;
				else
					startNode = nodes[index].next;
				
				if(nodes[index].next != RX_LIST_END)	//Not last.
					nodes[nodes[index].next].prev = nodes[index].prev;
				else
					endNode = nodes[index].prev;

				numNodes--;
				freeList[numFree++] = index;

				return true;
			}

			int	GetNextFreeIndex(void)	const
			{
				if(! numFree)
					return -1;

				return	freeList[numFree-1];
			}

			
			//Performs a proper lookup by traversing the list.
			genericValue	&operator	[](int index)
			{
				unsigned int ind = startNode;

				for(int i=0; i<index; i++)
				{
					ind = nodes[ind].next;
					if(ind == RX_LIST_END)
						return nodes[ind].value;
				}

				return nodes[ind].value;
			}

			/*!
				Simply resets the start and end node values to indicate an
				empty list without actually releasing memory. Useful when
				frequent reuse is required.
			*/
			void	Reset(void)
			{
				numFree += numNodes;
				numNodes = 0;
				startNode = endNode = RX_LIST_BEGIN;
			}


			//Copy constructor.
			SortedList(const SortedList &lst)
			{
				*this = lst;
			}

			SortedList	&operator =(const SortedList &lst)
			{
				if(nodes)
					delete	[]nodes;
				if(freeList)
					delete	[]freeList;

				numNodes	= lst.numNodes;
				startNode	= lst.startNode;
				endNode		= lst.endNode;
				autoGrow	= lst.autoGrow;
				numFree		= lst.numFree;

				unsigned int size = lst.numNodes + lst.numFree;

				//Allocate the Node list.
				if(! (nodes = new Node[size]) || ! (freeList = new unsigned int[size]) )
				{
					LOG.LogError( LString("Failed to allocate node/freelist array of size[%u].", size) );

					if(nodes)
						delete	[]nodes;

					nodes = NULL;
					freeList = NULL;
					return *this;
				}


				//Copy the data.
				for(unsigned int i=0; i<size; i++)
				{
					nodes[i]	= lst.nodes[i];
					freeList[i]	= lst.freeList[i];
				}

				return *this;
			}



		private:
			unsigned int	*freeList;			//!<List of free indices.
			unsigned int	numFree;			//!<Number of available indices.

		};






		/*!
			A linked list implementation using static arrays for quick insertion and
			traversal. Lookup/deletion isn't very fast.
		*/
		template	<class genericValue>
		class	List : public	Container<char, genericValue>		//A dummy key.
		{
		public:

			bool	Init(unsigned int size)
			{
				freeList = new unsigned int[size];

				if(! freeList)
				{
					LOG.LogError( LString("Failed to initialize freeList of size[%d].", size) );
					return false;
				}

				nodes = new Node[size];

				if(! nodes)
				{
					LOG.LogError( LString("Failed to initialize nodes of size[%d].", size) );

					delete	[]freeList;
					freeList = NULL;

					return false;
				}

				numFree = size;
				numNodes = 0;
				startNode = endNode = RX_LIST_BEGIN;

				//Set initial freeList values.
				for(unsigned int i=0; i<numFree; i++)
					freeList[i] = size-1 - i;

				return true;
			}

			void	Kill(void)
			{
				if(freeList)
				{
					delete	[]freeList;
					freeList = NULL;
				}

				if(nodes)
				{
					delete	[]nodes;
					nodes = NULL;
				}

				numFree = numNodes = 0;
				startNode = endNode = RX_LIST_BEGIN;
			}

			List(void)	: freeList(NULL)
			{
				Init(RX_LIST_DEFAULT_SIZE);
				autoGrow = true;
			}

			List(unsigned int size)
			{
				Init(size);
				autoGrow = true;
			}

			~List()
			{	Kill();	}


			/*!
				Adds a given value to the list. Automatically reallocates extra memory 
				in case the list is full. The increase is 2X each time.

				@param	value	The value to insert.
				@param	index	If not NULL then the index where the element is inserted 
								is stored here.
			*/
			bool	AddNode(genericValue &value, int *index = NULL)
			{
				if(index)
					*index = -1;

				if(! numFree)
				{
					if(! autoGrow)	//Cannot increase allocation.
						return false;

					//Need to increase allocation.
					unsigned int	*newFL;
					Node			*newNodes;
					unsigned int	size = numFree + numNodes;
					unsigned int	newSize = size * 2;		//We exponentially increase by factor of 2 each time.

					newFL = new unsigned int[newSize];

					if(! newFL)
					{
						LOG.LogError( LString("Failed to initialize new freeList of size[%d].", newSize) );
						return false;
					}

					newNodes = new Node[newSize];

					if(! newNodes)
					{
						LOG.LogError( LString("Failed to initialize new node list of size[%d].", newSize) );

						delete	[]newFL;
						newFL = NULL;

						return false;
					}
			

					//Copy the nodes.
					for(unsigned int i=0; i<size; i++)
						newNodes[i] = nodes[i];

					//And now the free list.
					for(unsigned int i=0; i<numFree; i++)
						newFL[i] = freeList[i];

					//Also add in the extra freelist entries.
					for(unsigned int i=numFree, j=size; j<newSize; i++, j++)
						newFL[i] = j;

					numFree += size;
					size = newSize;
					
					delete	[]freeList;
					delete	[]nodes;

					freeList = newFL;
					nodes = newNodes;
				}


				numFree--;
				numNodes++;
				Node	&node = nodes[freeList[numFree]];

				node.key = 'x';
				node.value = value;

				if(index)
					*index = (int) (freeList[numFree]);

				if(numNodes == 1)		//First element.
				{
					node.prev = RX_LIST_BEGIN;
					node.next = RX_LIST_END;
					startNode = freeList[numFree];
					endNode = freeList[numFree];

					return true;
				}

				//Insert at the end.
				node.prev = endNode;
				nodes[endNode].next = freeList[numFree];

				node.next = RX_LIST_END;
				endNode = freeList[numFree];

				return true;
			}

			/*!
				Traverses the list to find the element at index 'index' and deletes it.
			*/
			bool	DeleteWithLookup(int index)
			{
				if(index < 0 || index >= (int)numNodes)
					return false;

				unsigned int ind = startNode;

				for(int i=0; i<index; i++)
				{
					ind = nodes[ind].next;
					if(ind == RX_LIST_END)
						return false;	//End reached and not found.
				}

				return	Delete((int) ind);
			}


			/*!
				Deletes the element at given raw index.
			*/
			bool	Delete(int index)
			{
				if(index < 0)
					return false;

				if(nodes[index].prev != RX_LIST_BEGIN)	//Not first.
					nodes[nodes[index].prev].next = nodes[index].next;
				else
					startNode = nodes[index].next;
				
				if(nodes[index].next != RX_LIST_END)	//Not last.
					nodes[nodes[index].next].prev = nodes[index].prev;
				else
					endNode = nodes[index].prev;

				numNodes--;
				freeList[numFree++] = index;

				return true;
			}
			
			int	GetNextFreeIndex(void)	const
			{
				if(! numFree)
					return -1;

				return	freeList[numFree-1];
			}

			genericValue	&operator	[](int index)
			{
				if(index < (int)numNodes)
					return nodes[index].value;

				Iterator	itr(*this);
				int		i = 0;
				for(; i<(int)numNodes && i<index; i++, itr++);

				if(i == index)
					return	itr.GetValue();

				//Undefined behavior.
				return nodes[index].value;
			}

			/*!
				Simply resets the start and end node values to indicate an
				empty list without actually releasing memory. Useful when
				frequent reuse is required.
			*/
			void	Reset(void)
			{
				numFree += numNodes;
				numNodes = 0;
				startNode = endNode = RX_LIST_BEGIN;
			}


			//Copy constructor.
			List(const List &lst)
			{
				*this = lst;
			}


			List	&operator =(const List &lst)
			{
				if(nodes)
					delete	[]nodes;

				numNodes	= lst.numNodes;
				startNode	= lst.startNode;
				endNode		= lst.endNode;
				autoGrow	= lst.autoGrow;
				numFree		= lst.numFree;

				unsigned int size = lst.numNodes + lst.numFree;

				//Allocate the Node list.
				if(! (nodes = new Node[size]) || ! (freeList = new unsigned int[size]) )
				{
					LOG.LogError( LString("Failed to allocate node/freelist array of size[%u].", size) );

					if(nodes)
						delete	[]nodes;

					nodes = NULL;
					freeList = NULL;
					return *this;
				}


				//Copy the data.
				for(unsigned int i=0; i<size; i++)
				{
					nodes[i]	= lst.nodes[i];
					freeList[i]	= lst.freeList[i];
				}

				return *this;
			}

		private:
			unsigned int	*freeList;			//!<List of free indices.
			unsigned int	numFree;			//!<Number of available indices.

		};
	}
}

#endif