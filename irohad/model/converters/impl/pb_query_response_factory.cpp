/**
 * Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "model/converters/pb_query_response_factory.hpp"
#include "model/converters/pb_transaction_factory.hpp"
#include "model/converters/pb_common.hpp"

namespace iroha {
  namespace model {
    namespace converters {

      nonstd::optional<protocol::QueryResponse>
      PbQueryResponseFactory::serialize(
          const std::shared_ptr<QueryResponse> query_response) const {
        nonstd::optional<protocol::QueryResponse> response = nonstd::nullopt;
        // TODO 26/09/17 grimadas: refactor #VARIANT
        if (instanceof <model::ErrorResponse>(*query_response)) {
          response = nonstd::make_optional<protocol::QueryResponse>();
          auto er = static_cast<model::ErrorResponse &>(*query_response);
          auto pb_er = serializeErrorResponse(er);
          response->mutable_error_response()->CopyFrom(pb_er);
        }
        if (instanceof <model::AccountAssetResponse>(*query_response)) {
          response = nonstd::make_optional<protocol::QueryResponse>();
          response->mutable_account_assets_response()->CopyFrom(
              serializeAccountAssetResponse(
                  static_cast<model::AccountAssetResponse &>(*query_response)));
        }
        if (instanceof <model::AccountDetailResponse>(*query_response)) {
          response = nonstd::make_optional<protocol::QueryResponse>();
          response->mutable_account_detail_response()->CopyFrom(
            serializeAccountDetailResponse(
              static_cast<model::AccountDetailResponse &>(*query_response)));
        }
        if (instanceof <model::AccountResponse>(*query_response)) {
          response = nonstd::make_optional<protocol::QueryResponse>();
          response->mutable_account_response()->CopyFrom(
              serializeAccountResponse(
                  static_cast<model::AccountResponse &>(*query_response)));
        }
        if (instanceof <model::SignatoriesResponse>(*query_response)) {
          response = nonstd::make_optional<protocol::QueryResponse>();
          response->mutable_signatories_response()->CopyFrom(
              serializeSignatoriesResponse(
                  static_cast<model::SignatoriesResponse &>(*query_response)));
        }
        if (instanceof <model::TransactionsResponse>(*query_response)) {
          response = nonstd::make_optional<protocol::QueryResponse>();
          response->mutable_transactions_response()->CopyFrom(
              serializeTransactionsResponse(
                  static_cast<model::TransactionsResponse &>(*query_response)));
        }
        if (instanceof <model::AssetResponse>(*query_response)) {
          response = nonstd::make_optional<protocol::QueryResponse>();
          response->mutable_asset_response()->CopyFrom(serializeAssetResponse(
              static_cast<model::AssetResponse &>(*query_response)));
        }
        if (instanceof <model::RolesResponse>(*query_response)) {
          response = nonstd::make_optional<protocol::QueryResponse>();
          response->mutable_roles_response()->CopyFrom(serializeRolesResponse(
              static_cast<model::RolesResponse &>(*query_response)));
        }
        if (instanceof <model::RolePermissionsResponse>(*query_response)) {
          response = nonstd::make_optional<protocol::QueryResponse>();
          response->mutable_role_permissions_response()->CopyFrom(
              serializeRolePermissionsResponse(
                  static_cast<model::RolePermissionsResponse &>(
                      *query_response)));
        }

        return response;
      }

      protocol::Account PbQueryResponseFactory::serializeAccount(
          const model::Account &account) const {
        protocol::Account pb_account;
        pb_account.set_quorum(account.quorum);
        pb_account.set_account_id(account.account_id);
        pb_account.set_domain_id(account.domain_id);
        pb_account.set_json_data(account.json_data);
        return pb_account;
      }

      model::Account PbQueryResponseFactory::deserializeAccount(
          const protocol::Account &pb_account) const {
        model::Account res;
        res.account_id = pb_account.account_id();
        res.quorum = pb_account.quorum();
        res.domain_id = pb_account.domain_id();
        res.json_data = pb_account.json_data();
        return res;
      }

      protocol::AccountResponse
      PbQueryResponseFactory::serializeAccountResponse(
          const model::AccountResponse &accountResponse) const {
        protocol::AccountResponse pb_response;
        pb_response.mutable_account()->CopyFrom(
            serializeAccount(accountResponse.account));
        for (auto role : accountResponse.roles){
          pb_response.add_account_roles(role);
        }
        return pb_response;
      }

      model::AccountResponse PbQueryResponseFactory::deserializeAccountResponse(
          const protocol::AccountResponse pb_response) const {
        model::AccountResponse accountResponse;
        accountResponse.account = deserializeAccount(pb_response.account());
        return accountResponse;
      }

      protocol::AccountAsset PbQueryResponseFactory::serializeAccountAsset(
          const model::AccountAsset &account_asset) const {
        protocol::AccountAsset pb_account_asset;
        pb_account_asset.set_account_id(account_asset.account_id);
        pb_account_asset.set_asset_id(account_asset.asset_id);
        auto pb_balance = pb_account_asset.mutable_balance();
        pb_balance->CopyFrom(serializeAmount(account_asset.balance));
        return pb_account_asset;
      }

      model::AccountAsset PbQueryResponseFactory::deserializeAccountAsset(
          const protocol::AccountAsset &account_asset) const {
        model::AccountAsset res;
        res.account_id = account_asset.account_id();
        res.balance = deserializeAmount(account_asset.balance());
        res.asset_id = account_asset.asset_id();
        return res;
      }

      protocol::AccountAssetResponse
      PbQueryResponseFactory::serializeAccountAssetResponse(
          const model::AccountAssetResponse &accountAssetResponse) const {
        protocol::AccountAssetResponse pb_response;
        auto pb_account_asset = pb_response.mutable_account_asset();
        pb_account_asset->set_asset_id(
            accountAssetResponse.acct_asset.asset_id);
        pb_account_asset->set_account_id(
            accountAssetResponse.acct_asset.account_id);
        auto pb_amount = pb_account_asset->mutable_balance();
        pb_amount->CopyFrom(serializeAmount(accountAssetResponse.acct_asset.balance));
        return pb_response;
      }

      model::AccountAssetResponse
      PbQueryResponseFactory::deserializeAccountAssetResponse(
          const protocol::AccountAssetResponse &account_asset_response) const {
        model::AccountAssetResponse res;
        res.acct_asset.balance =
            deserializeAmount(account_asset_response.account_asset().balance());
        res.acct_asset.account_id =
            account_asset_response.account_asset().account_id();
        res.acct_asset.asset_id =
            account_asset_response.account_asset().asset_id();
        return res;
      }

      protocol::AccountDetailResponse
      PbQueryResponseFactory::serializeAccountDetailResponse(
        const model::AccountDetailResponse &accountDetailResponse) const {
        protocol::AccountDetailResponse pb_response;
        pb_response.set_detail(accountDetailResponse.detail);
        return pb_response;
      }

      model::AccountDetailResponse
      PbQueryResponseFactory::deserializeAccountDetailResponse(
        const protocol::AccountDetailResponse &account_detail_response) const {
        model::AccountDetailResponse res;
        res.detail = account_detail_response.detail();
        return res;
      }


        protocol::SignatoriesResponse
      PbQueryResponseFactory::serializeSignatoriesResponse(
          const model::SignatoriesResponse &signatoriesResponse) const {
        protocol::SignatoriesResponse pb_response;

        for (auto key : signatoriesResponse.keys) {
          pb_response.add_keys(key.data(), key.size());
        }
        return pb_response;
      }

      model::SignatoriesResponse
      PbQueryResponseFactory::deserializeSignatoriesResponse(
          const protocol::SignatoriesResponse &signatoriesResponse) const {
        model::SignatoriesResponse res{};
        for (const auto &key : signatoriesResponse.keys()) {
          pubkey_t pubkey;
          std::copy(key.begin(), key.end(), pubkey.begin());
          res.keys.push_back(pubkey);
        }
        return res;
      }

      protocol::AssetResponse PbQueryResponseFactory::serializeAssetResponse(
          const model::AssetResponse &response) const {
        protocol::AssetResponse res;
        auto asset = res.mutable_asset();
        asset->set_asset_id(response.asset.asset_id);
        asset->set_domain_id(response.asset.domain_id);
        asset->set_precision(response.asset.precision);
        return res;
      }

      model::AssetResponse PbQueryResponseFactory::deserializeAssetResponse(
          const protocol::AssetResponse &response) const {
        model::AssetResponse res;
        auto asset = response.asset();
        res.asset =
            Asset(asset.asset_id(), asset.domain_id(), asset.precision());
        return res;
      }

      protocol::RolesResponse PbQueryResponseFactory::serializeRolesResponse(
          const model::RolesResponse &response) const {
        protocol::RolesResponse res;
        for (auto role : response.roles) {
          res.add_roles(role);
        }
        return res;
      }

      model::RolesResponse PbQueryResponseFactory::deserializeRolesResponse(
          const protocol::RolesResponse &response) const {
        model::RolesResponse res{};
        std::copy(response.roles().begin(), response.roles().end(),
                  res.roles.begin());
        return res;
      }

      protocol::RolePermissionsResponse
      PbQueryResponseFactory::serializeRolePermissionsResponse(
          const model::RolePermissionsResponse &response) const {
        protocol::RolePermissionsResponse res;
        for (auto perm : response.role_permissions) {
          res.add_permissions(perm);
        }
        return res;
      }

      model::RolePermissionsResponse
      PbQueryResponseFactory::deserializeRolePermissionsResponse(
          const protocol::RolePermissionsResponse &response) const {
        model::RolePermissionsResponse res;
        std::copy(response.permissions().begin(), response.permissions().end(),
                  res.role_permissions.begin());
        return res;
      }

      protocol::TransactionsResponse
      PbQueryResponseFactory::serializeTransactionsResponse(
          const model::TransactionsResponse &transactionsResponse) const {
        PbTransactionFactory pb_transaction_factory;

        // converting observable to the vector using reduce
        return transactionsResponse.transactions
            .reduce(protocol::TransactionsResponse(),
                    [&pb_transaction_factory](auto &&response, auto tx) {
                      response.add_transactions()->CopyFrom(
                          pb_transaction_factory.serialize(tx));
                      return response;
                    },
                    [](auto &&response) { return response; })
            .as_blocking()  // we need to wait when on_complete happens
            .first();
      }

      protocol::ErrorResponse PbQueryResponseFactory::serializeErrorResponse(
          const model::ErrorResponse &errorResponse) const {
        protocol::ErrorResponse pb_response;
        switch (errorResponse.reason) {
          case ErrorResponse::NO_ASSET:
            pb_response.set_reason(protocol::ErrorResponse::NO_ASSET);
            break;
          case ErrorResponse::STATELESS_INVALID:
            pb_response.set_reason(protocol::ErrorResponse::STATELESS_INVALID);
            break;
          case ErrorResponse::STATEFUL_INVALID:
            pb_response.set_reason(protocol::ErrorResponse::STATEFUL_INVALID);
            break;
          case ErrorResponse::NO_ACCOUNT:
            pb_response.set_reason(protocol::ErrorResponse::NO_ACCOUNT);
            break;
          case ErrorResponse::NO_ACCOUNT_ASSETS:
            pb_response.set_reason(protocol::ErrorResponse::NO_ACCOUNT_ASSETS);
            break;
          case ErrorResponse::NO_ACCOUNT_DETAIL:
            pb_response.set_reason(protocol::ErrorResponse::NO_ACCOUNT_DETAIL);
            break;
          case ErrorResponse::NO_SIGNATORIES:
            pb_response.set_reason(protocol::ErrorResponse::NO_SIGNATORIES);
            break;
          case ErrorResponse::NOT_SUPPORTED:
            pb_response.set_reason(protocol::ErrorResponse::NOT_SUPPORTED);
            break;
          case ErrorResponse::NO_ROLES:
            pb_response.set_reason(protocol::ErrorResponse::NO_ROLES);
            break;
        }
        return pb_response;
      }
    }  // namespace converters
  }    // namespace model
}  // namespace iroha
