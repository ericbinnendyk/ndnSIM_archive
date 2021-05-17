// this is a draft of how the neural network strategy should collect data about interest interface and round-trip time.

// probability of forwarding to each interface
interface_probabilities = {0.25, 0.25, 0.25, 0.25};
// vector of pairs of interface choice, RTT delay
interface_delays = {};
// should we compete these or feed the data in raw?
interface_average_delays = {0, 0, 0, 0};

//dict mapping interest names to forwarding choice
std::map<std::string, int> forward_choice;
//dict mapping interest names to RTT delays
std::map<std::string, double> rtt_delays;

on_interest(interest)
{
    std::string interest_name = interest->toUrl???
    int choice = rand_choice(interface_probabilities);
    interface = choice_to_interface(choice);
    forward_choice[interest_name] = choice;
    interface.forward(interest);
}

on_satisfied_data(data)
{
    std::string data_name = data.name();
    double delay_time = // magically get round-trip time of data
    ;
    int choice = foward_choice[data_name];
    interface_delays.push_back(std::pair<int, double>(choice, delay_time));
}

print_data()
{
    FILE *f = fopen("nn-input.txt", "w");
    for (auto pair : interface_delays) {
        fprintf(f, "%d, %f\n", pair.first, pair.second);
    }
    f.close();
}