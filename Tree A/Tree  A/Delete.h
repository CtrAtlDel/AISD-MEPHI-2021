#define SIZE 4

int Delete(Tree* tree, Node** root, char* key, int release) //еще прошивка
{
	Node* node_search = NULL;
	Node* for_tread = NULL;
	Node* ptr = NULL;
	Node* last_right = NULL;
	Node* parent = NULL;
	Node* temp = NULL;
	if (strlen(key) > SIZE)
	{
		return -2;
	}
	for_tread = *root;
	node_search = NULL;//root
	while (for_tread)
	{
		if (strcmp(key, for_tread->key) > 0)
		{
			last_right = for_tread;//последний правый указатель
			for_tread = for_tread->right;
			continue;
		}
		if (strcmp(key, for_tread->key) < 0)
		{

			for_tread = for_tread->left;
			continue;
		}
		if (strcmp(key, for_tread->key) == 0)
		{
			node_search = for_tread;
			break; //нашли нужный узел
		}
	}
	if (node_search == NULL) { return -1; }
	if (release == -1)
	{
		if ((node_search->left == NULL) && (node_search->right == NULL)) //нет поддеревьев done.
		{
			ptr = node_search->parent;
			if (ptr == NULL)
			{
				Delete_node(node_search);
				tree->root = NULL;
				return 1;
			}
			if (node_search == ptr->right)
			{
				ptr->right = NULL;
				ptr->next = node_search->next;
			}
			if (node_search == ptr->left)
			{
				ptr->left = NULL;
				if (last_right != NULL)
				{
					last_right->next = node_search->next;
				}
			}
			Delete_node(node_search);
			return 1;
		}
		if (node_search->left == NULL && node_search->right != NULL) // поддерево справа 
		{
			ptr = node_search->parent;
			if (ptr == NULL)
			{
				temp = node_search->right;
				//temp->next = node_search->next;
				temp->parent = node_search->parent;
				Delete_node(node_search);
				tree->root = temp;
				return 1;
			}
			if (node_search == ptr->right)
			{
				ptr->right = node_search->right;

				if (node_search->right == Get_min_node(node_search->right))
				{
					ptr->next = node_search->right;
				}
				else
				{
					temp = Get_min_node(node_search);
					temp->next = node_search->next;
					Delete_node(node_search);
					return 1;
				}
			}
			if (node_search == ptr->left)
			{
				ptr->left = node_search->right;
				if (last_right)
				{
					last_right->next = node_search->next;
					Delete_node(node_search);
					return 1;
				}
			}
		}
		if (node_search->left != NULL && node_search->right == NULL)
		{
			ptr = node_search->parent;
			if (ptr == NULL)
			{
				temp = node_search->left;
				temp->parent = node_search->parent;
				temp->next = node_search->next;
				Delete_node(node_search);
				tree->root = temp;
				return 1;
			}
			if (node_search == ptr->right)
			{
				ptr->right = node_search->left;

				temp = Get_max_node(node_search->left);
				temp->next = node_search->next;
				Delete_node(node_search);
				return 1;

			}
			if (node_search == ptr->left)
			{
				ptr->left = node_search->left;

				temp = Get_max_node(node_search->left);
				temp->next = node_search->next;
				Delete_node(node_search);
				return 1;
			}
		}
		if (node_search->left != NULL && node_search->right != NULL)
		{
			temp = Get_max_node(node_search->left);
			ptr = node_search->parent;
			if (ptr == NULL)
			{
				if (node_search->left == temp)
				{
					temp->next = node_search->next;
					temp->parent = node_search->parent;
					temp->right = node_search->right;
					Delete_node(node_search);
					tree->root = temp;
					return 1;
				}

				temp->next = node_search->next;
				temp->parent->right = temp->left;
				temp->parent = NULL;

				temp->right = node_search->right;
				temp->left = node_search->left;
				Delete_node(node_search);
				tree->root = temp;
				return 1;
			}
			if (node_search == ptr->left)
			{
				if (temp->parent != node_search)
				{
					temp->parent->right = temp->left;
					temp->parent = node_search->parent;
					node_search->parent->left = temp;
					temp->left = node_search->left;
					temp->right = node_search->right;
					temp->next = node_search->next;
					Delete_node(node_search);
					return 1;
				}
				else
				{
					node_search->parent->left = temp;

					temp->right = node_search->right;
					temp->parent = node_search->parent;
					temp->next = node_search->next;
					Delete_node(node_search);
					return 1;
				}
			}
			if (node_search == ptr->right)
			{
				if (temp->parent != node_search)
				{
					temp->parent->left = temp->right;
					temp->next = node_search->next;
					temp->parent = node_search->parent->right;
					node_search->parent->right = temp;
					temp->left = node_search->left;
					temp->right = node_search->right;
					Delete_node(node_search);
					return 1;
				}
				else
				{
					temp->next = node_search->next;
					node_search->parent->right = temp;
					temp->right = node_search->right;
					temp->parent = node_search->parent;

					Delete_node(node_search);
					return 1;

				}
			}
			return 1;
		}
	}
	else
	{
		Node* temp = NULL;
		Node* prev = NULL;
		int flag = 0;
		temp = node_search;
		while (temp != NULL)
		{
			if (temp->str.realese == release)
			{
				flag = 1;
				break;
			}
			temp = temp->str.next_inf;
		}
		if (temp == NULL)
		{
			return -3; //Ќе нашли версию
		}
		if (temp == node_search)
		{
			flag = Delete(tree, &(tree->root), key, -1);
			return flag;
		}
		else
		{
			prev = node_search;
			while (prev->str.next_inf != temp)
			{
				prev = prev->str.next_inf;
			}
			prev = temp->str.next_inf;
			Delete_node(temp);
			return 1;//well done
		}

	}
}
