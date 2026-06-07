// Copyright 2022 NNTU-CS
#include <algorithm>
#include <vector>
#include "tree.h"

PMTree::PMTree(const std::vector<char>& src) : base(src) {
    top = new Node(0);

    std::vector<char> sorted_src = src;
    std::sort(sorted_src.begin(), sorted_src.end());

    for (size_t i = 0; i < sorted_src.size(); ++i) {
        std::vector<char> leftover = sorted_src;
        leftover.erase(leftover.begin() + static_cast<std::vector<char>::difference_type>(i));
        Node* child = generate(leftover);
        child->sym = sorted_src[i];
        top->links.push_back(child);
    }
}

PMTree::~PMTree() {
    destroy(top);
}

PMTree::Node* PMTree::generate(const std::vector<char>& rest) {
    Node* cur = new Node(0);

    if (rest.empty()) return cur;

    for (size_t i = 0; i < rest.size(); ++i) {
        std::vector<char> leftover = rest;
        leftover.erase(leftover.begin() + static_cast<std::vector<char>::difference_type>(i));
        Node* child = generate(leftover);
        child->sym = rest[i];
        cur->links.push_back(child);
    }
    return cur;
}

void PMTree::destroy(Node* ptr) {
    if (!ptr) return;
    for (auto child : ptr->links) {
        destroy(child);
    }
    delete ptr;
}

void traverse(PMTree::Node* cur, std::vector<char>& buf,
    std::vector<std::vector<char>>& out, int level, int limit) {
    if (level == limit) {
        out.push_back(buf);
        return;
    }
    for (auto nxt : cur->links) {
        buf.push_back(nxt->sym);
        traverse(nxt, buf, out, level + 1, limit);
        buf.pop_back();
    }
}

std::vector<std::vector<char>> getAllPerms(PMTree& obj) {
    std::vector<std::vector<char>> out;
    std::vector<char> track;
    int total = static_cast<int>(obj.base.size());

    if (total == 0) return out;

    for (auto first : obj.top->links) {
        track.push_back(first->sym);
        traverse(first, track, out, 1, total);
        track.pop_back();
    }
    return out;
}

std::vector<char> getPerm1(PMTree& obj, int pos) {
    auto full = getAllPerms(obj);
    if (pos <= 0 || pos > static_cast<int>(full.size())) return {};
    return full[pos - 1];
}

size_t fact(int n) {
    size_t res = 1;
    for (int i = 2; i <= n; ++i) res *= i;
    return res;
}

std::vector<char> getPerm2(PMTree& obj, int pos) {
    int total = static_cast<int>(obj.base.size());
    if (pos <= 0 || static_cast<size_t>(pos) > fact(total)) return {};

    std::vector<char> available = obj.base;
    std::sort(available.begin(), available.end());

    std::vector<char> result;
    size_t remainder = static_cast<size_t>(pos - 1);

    for (int step = total; step > 0; --step) {
        size_t block = fact(step - 1);
        size_t idx = remainder / block;
        remainder %= block;

        if (idx >= available.size()) return {};
        result.push_back(available[idx]);
        available.erase(available.begin() + static_cast<std::vector<char>::difference_type>(idx));
    }
    return result;
}
