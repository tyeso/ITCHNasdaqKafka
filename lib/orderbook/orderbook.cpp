#include "orderbook.h"

int itchkafka::Orderbook::getIndex(char side, price_t price)
{
    switch(side){
        case 'S': {
            auto it = std::find_if(ask_book.begin(), ask_book.end(),
              [&](const auto& pair) { return pair.first == price; });
            int position = std::distance(this->ask_book.begin(), it);
            return position;
        }
        case 'B': {
            auto it = std::find_if(bid_book.begin(), bid_book.end(),
              [&](const auto& pair) { return pair.first == price; });
            int position = std::distance(this->bid_book.begin(), it);
            return position;
        }
        default: {
            std::cerr << "Invalid side: " << side << std::endl;
            return 0;
        }
    }
}

timestamp_t itchkafka::Orderbook::getLuts()
{
    return this->luts;
}

std::vector<std::vector<uint32_t>> getLOB(int level)
{

}

void itchkafka::Orderbook::buildLOB(int level)
{
    orderId_t order_id;
    price_t price;
    quantity_t quantity;

    // TODO: do not clear ask book and bid book every time we call it, rather update it

    this->ask_book.clear();
    this->bid_book.clear();

    for (auto const& it : this->ask_orders)
    {
        order_id = it.first;
        price = it.second.first;
        quantity = it.second.second;
        if (quantity > 0) {this->ask_book[price] += quantity;}
    }
    for (auto const& it : this->bid_orders)
    {
        order_id = it.first;
        price = it.second.first;
        quantity = it.second.second;
        if (quantity > 0) {this->bid_book[price] += quantity;}
    }
}

void itchkafka::Orderbook::printLOB(int level)
{
    this->buildLOB(level);
    std::cout << "----------------------------------------" << std::endl;
    int count = this->ask_book.size();
    for (auto it=this->ask_book.rbegin(); it!=this->ask_book.rend(); ++it)
    {
        if (count > level) {count--; continue;}
        std::cout << "Level " << count << " " << it->first << " : " << it->second << std::endl;
        count--;
    }
    std::cout << "----------------------------------------" << std::endl;
    count = 1;
    for (auto it=this->bid_book.begin(); it!=this->bid_book.end(); ++it)
    {
        if (count > level) {break;}
        std::cout << "Level " << count << " " << it->first << " : " << it->second << std::endl;
        count++;
    }
    std::cout << "----------------------------------------" << std::endl;
}

std::vector<uint32_t> itchkafka::Orderbook::getOHLCVT(timestamp_t curr_timestamp)
{

}

bool itchkafka::Orderbook::buildOHLCVT(timestamp_t curr_timestamp)
{
    timestamp_t timestamp;
    price_t open_price = 0;
    price_t high_price = 0;
    price_t low_price = 1e9;    // set as large number, as we are to keep cheaper prices
    price_t close_price = 0;
    quantity_t aev = 0;
    quantity_t bev = 0;

    timestamp_t open_ts;
    timestamp_t close_ts;
    timestamp_t tmp_timestamp;
    price_t tmp_price;
    quantity_t tmp_quantity;

    uint64_t sec_in_ns = 1e9;
    
    // ask side
    for(auto const& it : this->ask_trades)
    {
        tmp_timestamp = it.first;
        close_ts = tmp_timestamp;
        tmp_price = it.second.first;
        tmp_quantity = it.second.second;

        if ((tmp_timestamp >= curr_timestamp - sec_in_ns) && (tmp_timestamp <= curr_timestamp))
        {
            // price updates
            if (open_price == 0) {open_price = tmp_price; open_ts = tmp_timestamp;}
            if (tmp_price > high_price) {high_price = tmp_price;}
            if (tmp_price < low_price) {low_price = tmp_price;}
            close_price = tmp_price;

            // quantity updates
            aev += tmp_quantity;
        }
    }

    // bid side
    for(auto const& it : this->bid_trades)
    {
        tmp_timestamp = it.first;
        tmp_price = it.second.first;
        tmp_quantity = it.second.second;

        if ((tmp_timestamp >= curr_timestamp - sec_in_ns) && (tmp_timestamp <= curr_timestamp))
        {
            // price updates
            if (open_price == 0) {open_price = tmp_price; open_ts = tmp_timestamp;}
            else if (tmp_timestamp < open_ts) {open_price = tmp_price; open_ts = tmp_timestamp;}

            if (tmp_price > high_price) {high_price = tmp_price;}
            if (tmp_price < low_price) {low_price = tmp_price;}
            if (tmp_timestamp > close_ts) {close_price = tmp_price; close_ts = tmp_timestamp;}

            // quantity updates
            bev += tmp_quantity;
        }
    }

    if (open_price != 0 && high_price != 0 && low_price != 1e9 && close_price != 0)
    {
        this->t_ohlcvv[curr_timestamp] = {open_price, high_price, low_price, close_price, aev, bev};
        return true;
    }
    else {return false;}
}

void itchkafka::Orderbook::printOHLCVT(timestamp_t curr_timestamp)
{
    bool ohlcvt_built = this->buildOHLCVT(curr_timestamp);
    if (!ohlcvt_built) {return;}
    auto pos = this->t_ohlcvv.find(curr_timestamp);
    if (pos == this->t_ohlcvv.end()) { std::cerr << "Cannot find ohlcvv data at : " << curr_timestamp << std::endl;}
    else {
        std::cout << "timestamp: " << pos->first << "\nOHLCVV:\n";
        for (uint32_t it : pos->second)
        {
            std::cout << it << " ";
        }
        std::cout << std::endl;
    }
}

void itchkafka::Orderbook::updateMessageVariant(itchkafka::variant_message variant_message)
{
    std::visit([this](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, itchkafka::SeqSymbolDirectoryMessage>)
            this->updateMessage(arg);
        else if constexpr (std::is_same_v<T, itchkafka::SeqAddOrderMessage>)
            this->updateMessage(arg);
        else if constexpr (std::is_same_v<T, itchkafka::SeqAddAttributedOrderMessage>)
            this->updateMessage(arg);
        else if constexpr (std::is_same_v<T, itchkafka::SeqOrderExecutedMessage>)
            this->updateMessage(arg);
        else if constexpr (std::is_same_v<T, itchkafka::SeqOrderExecutedWithPriceMessage>)
            this->updateMessage(arg);
        else if constexpr (std::is_same_v<T, itchkafka::SeqOrderCancelMessage>)
            this->updateMessage(arg);
        else if constexpr (std::is_same_v<T, itchkafka::SeqOrderDeleteMessage>)
            this->updateMessage(arg);
        else if constexpr (std::is_same_v<T, itchkafka::SeqReplaceOrderMessage>)
            this->updateMessage(arg);
        else if constexpr (std::is_same_v<T, itchkafka::SeqTradeMessage>)
            this->updateMessage(arg);
        else if constexpr (std::is_same_v<T, itchkafka::SeqCrossTradeMessage>)
            this->updateMessage(arg);
        else if constexpr (std::is_same_v<T, itchkafka::SeqBrokenTradeMessage>)
            this->updateMessage(arg);
    }, variant_message);
}

void itchkafka::Orderbook::updateMessage(SeqSymbolDirectoryMessage message)
{
    // std::cout << "in updateMessage: SeqAddOrderMessage" << std::endl;
    this->sym_locate = message.symbolLocate;
    this->luts = message.uniqueTimestamp;
}

void itchkafka::Orderbook::updateMessage(SeqAddOrderMessage message)
{
    // std::cout << "in updateMessage: SeqAddOrderMessage" << std::endl;
    switch(message.side){
        case 'S': {
            this->ask_orders[message.orderId] = std::make_pair(message.price, message.quantity);
            break;
        }
        case 'B': {
            this->bid_orders[message.orderId] = std::make_pair(message.price, message.quantity);
            break;
        }
    }
    this->luts = message.uniqueTimestamp;
}

void itchkafka::Orderbook::updateMessage(SeqAddAttributedOrderMessage message)
{
    // std::cout << "in updateMessage: SeqAddAttributedOrderMessage" << std::endl;
    switch(message.side){
        case 'S': {
            this->ask_orders[message.orderId] = std::make_pair(message.price, message.quantity);
        }
        case 'B': {
            this->bid_orders[message.orderId] = std::make_pair(message.price, message.quantity);
        }
    }
    this->luts = message.uniqueTimestamp;
}

void itchkafka::Orderbook::updateMessage(SeqOrderExecutedMessage message)
{
    // std::cout << "in updateMessage: SeqOrderExecutedMessage" << std::endl;
    if(this->ask_orders.find(message.orderId) != this->ask_orders.end())
    {
        this->ask_orders[message.orderId].second -= message.quantity;
    }
    else if (this->bid_orders.find(message.orderId) != this->bid_orders.end())
    {
        this->bid_orders[message.orderId].second -= message.quantity;
    }
    else
    {
        std::cerr << "OrderID not found: " << message.orderId << std::endl;
    }
    this->luts = message.uniqueTimestamp;
}

void itchkafka::Orderbook::updateMessage(SeqOrderExecutedWithPriceMessage message)
{
    // std::cout << "in updateMessage: SeqOrderExecutedWithPriceMessage" << std::endl;
    if(this->ask_orders.find(message.orderId) != this->ask_orders.end())
    {
        this->ask_orders[message.orderId].second -= message.quantity;
    }
    else if (this->bid_orders.find(message.orderId) != this->bid_orders.end())
    {
        this->bid_orders[message.orderId].second -= message.quantity;
    }
    else
    {
        std::cerr << "OrderID not found: " << message.orderId << std::endl;
    }
    this->luts = message.uniqueTimestamp;
}

void itchkafka::Orderbook::updateMessage(SeqOrderCancelMessage message)
{
    // std::cout << "in updateMessage: SeqOrderCancelMessage" << std::endl;
    if(this->ask_orders.find(message.orderId) != this->ask_orders.end())
    {
        this->ask_orders[message.orderId].second -= message.quantity;
    }
    else if (this->bid_orders.find(message.orderId) != this->bid_orders.end())
    {
        this->bid_orders[message.orderId].second -= message.quantity;
    }
    else
    {
        std::cerr << "OrderID not found: " << message.orderId << std::endl;
    }
    this->luts = message.uniqueTimestamp;
}

void itchkafka::Orderbook::updateMessage(SeqOrderDeleteMessage message)
{
    // std::cout << "in updateMessage: SeqOrderDeleteMessage" << std::endl;
    if(this->ask_orders.find(message.orderId) != this->ask_orders.end())
    {
        this->ask_orders.erase(this->ask_orders.find(message.orderId));
    }
    else if (this->bid_orders.find(message.orderId) != this->bid_orders.end())
    {
        this->bid_orders.erase(this->bid_orders.find(message.orderId));
    }
    else
    {
        std::cerr << "OrderID not found: " << message.orderId << std::endl;
    }
    this->luts = message.uniqueTimestamp;
}

void itchkafka::Orderbook::updateMessage(SeqReplaceOrderMessage message)
{
    // std::cout << "in updateMessage: SeqReplaceOrderMessage" << std::endl;
    // Delete previous order with OrderId
    char side;
    if(this->ask_orders.find(message.orderId) != this->ask_orders.end())
    {
        this->ask_orders.erase(this->ask_orders.find(message.orderId));
        side = 'S';
    }
    else if (this->bid_orders.find(message.orderId) != this->bid_orders.end())
    {
        this->bid_orders.erase(this->bid_orders.find(message.orderId));
        side = 'B';
    }
    else
    {
        std::cerr << "OrderID not found: " << message.orderId << std::endl;
    }

    // Add new order with newOrderId
    switch(side){
        case 'S': {
            this->ask_orders[message.newOrderId] = std::make_pair(message.price, message.quantity);
        }
        case 'B': {
            this->bid_orders[message.newOrderId] = std::make_pair(message.price, message.quantity);
        }
    }
    this->luts = message.uniqueTimestamp;
}

void itchkafka::Orderbook::updateMessage(SeqTradeMessage message)
{
    // std::cout << "in updateMessage: SeqTradeMessage" << std::endl;
    switch(message.side){
        case 'S': {
            if(this->ask_orders.find(message.orderId) != this->ask_orders.end())
            {
                this->ask_orders[message.orderId].second -= message.quantity;
            }
            this->ask_trades[message.uniqueTimestamp] = std::make_pair(message.price, message.quantity);
        }
        case 'B': {
            if(this->bid_orders.find(message.orderId) != this->bid_orders.end())
            {
                this->bid_orders[message.orderId].second -= message.quantity;
            }
            this->bid_trades[message.uniqueTimestamp] = std::make_pair(message.price, message.quantity);
        }
    }
    this->luts = message.uniqueTimestamp;
}

void itchkafka::Orderbook::updateMessage(SeqCrossTradeMessage message)
{
    // std::cout << "in updateMessage: SeqCrossTradeMessage" << std::endl;
    this->ask_trades[message.uniqueTimestamp] = std::make_pair(message.price, message.quantity);
    this->bid_trades[message.uniqueTimestamp] = std::make_pair(message.price, message.quantity);
    this->luts = message.uniqueTimestamp;
}

void itchkafka::Orderbook::updateMessage(SeqBrokenTradeMessage message)
{
    // std::cout << "in updateMessage: SeqBrokenTradeMessage" << std::endl;
    // TODO:
    this->luts = message.uniqueTimestamp;
}