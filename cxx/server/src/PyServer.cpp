#include "PyServer.h"

PyServer::PyServer(io_api::io_context &ctx, const ipv4::endpoint &ep)
    : st(&socket, [this] { on_write(); })
    , socket(ctx, ep, [] {}) {
    std::cerr << "PyServer booted" << std::endl;
}

PyServer::~PyServer() {}

void PyServer::submit_request(const std::string &s) {
    std::cerr << "Submitting request..." << std::endl;

    st.push_front(s);
}

void PyServer::on_write() {
    std::string send = st.get();

    std::cerr << "sending to pyserver...: " << send << std::endl;

    int r = socket.send(send.c_str(), send.size());
    if (r < 0) {
        if (errno == EINTR)
            return;
        IPV4_EXC();
    }

    if (r < send.size())
        st.push_back(send.substr(r));
}
