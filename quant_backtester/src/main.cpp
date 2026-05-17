#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "quant/core/experiment_config.h"
#include "quant/core/engine.h"
#include "quant/execution/execution_engine.h"
#include "quant/execution/order_manager.h"
#include "quant/portfolio/portfolio.h"
#include "quant/utils/event_logger.h"
#include "quant/utils/logger.h"

int main(int argc, char** argv) {
    using namespace quant;

    // Allow overriding the experiment config from CLI for quick scenario runs.
    const std::string default_cfg = "configs/experiments/default_experiment.json";
    const std::string cfg_path = (argc > 1) ? argv[1] : default_cfg;

    try {
        // 1) Load config and initialize structured logging.
        const ExperimentConfig cfg = load_experiment_config(cfg_path);
        StructuredLogger slog(cfg.structured_log_path, true);
        slog.info("Loading experiment config from: " + cfg_path);

        // 2) Resolve data source and construct strategy/execution modules.
        std::vector<Price> prices = resolve_prices(cfg);
        slog.info("Loaded " + std::to_string(prices.size()) + " price bars");

        auto strategy = build_strategy(cfg);
        const RiskLimits limits{
            cfg.risk_max_order_qty,
            cfg.risk_max_abs_position_per_instrument,
            cfg.risk_max_gross_notional,
        };
        OrderManager order_manager(cfg.lot_size, limits);
        ExecutionEngine execution_engine(
            SlippageModel(cfg.slippage_bps),
            FillSimulator{},
            TransactionCostModel(cfg.commission_per_share));
        Portfolio portfolio(cfg.starting_cash);
        EventLogger event_logger;

        // 3) Execute the backtest and print a user-facing summary.
        Engine engine(*strategy, order_manager, execution_engine, portfolio, &event_logger, cfg.artifacts_root);
        BacktestResult result = engine.run(prices, cfg.instrument);

        std::cout << result.report;
        std::cout << "Run ID: " << result.run_id << '\n';
        std::cout << "Run Directory: " << result.run_directory << '\n';
        std::cout << "Replay Log: " << result.replay_log_path << '\n';
        std::cout << "Logged events: " << result.event_log.size() << '\n';

        slog.info("Backtest completed. Run ID=" + result.run_id + ", events=" + std::to_string(result.event_log.size()));
        return 0;
    } catch (const std::exception& ex) {
        // Keep failure mode explicit for scripting and CI integration.
        std::cerr << "Backtester error: " << ex.what() << '\n';
        return 1;
    }
}
